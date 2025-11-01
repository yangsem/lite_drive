#include "net_engine_impl.h"
#include <new>
#include <memory>
#include <error_code.h>

namespace lite_drive
{
namespace net_engine
{

INetEngine* INetEngine::Create(logger::ILogger *pLogger)
{
    return new(std::nothrow) NetEngineImpl(pLogger);
}

void INetEngine::Destroy(INetEngine *pNetEngine)
{
    if (pNetEngine != nullptr)
    {
        delete pNetEngine;
    }
}

NetEngineImpl::NetEngineImpl(logger::ILogger *pLogger) : m_pLogger(pLogger)
{
}

NetEngineImpl::~NetEngineImpl()
{
    Stop();
    Exit();
}

int32_t NetEngineImpl::Init(utilities::IConfig *pConfig, ICallback *pGlobalCallback)
{
    if (pConfig == nullptr || pGlobalCallback == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid parameters");
        return ErrorCode::kInvalidParam;
    }

    m_pConfig = utilities::IConfig::Create();
    if (m_pConfig == nullptr || m_pConfig->Copy(pConfig) != ErrorCode::kSuccess)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kNoMemory, "Failed to copy config");
        return ErrorCode::kNoMemory;
    }

    try
    {
        m_strNetEngineName = m_pConfig->GetStr(config::kSection, config::kNetEngineName, default_value::kNetEngineName);
        uint32_t uIOThreadCount = m_pConfig->GetInt32(config::kSection, config::kIOThreadCount, default_value::kIOThreadCount);
        uIOThreadCount = std::max(uIOThreadCount, 1u);
        m_vecThIO.resize(uIOThreadCount);
        LOG_EVENT(m_pLogger, ErrorCode::kEvent, "{} io thread count: {}", m_strNetEngineName.c_str(), Wrap(uIOThreadCount));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kThrowException, "falied to init net engine");
        return ErrorCode::kThrowException;
    }
    m_pGlobalCallback = pGlobalCallback;
    return ErrorCode::kSuccess;
}

void NetEngineImpl::Exit()
{
    m_pLogger = nullptr;
    m_pGlobalCallback = nullptr;
    m_strNetEngineName.clear();
    m_vecThIO.clear();
    if (m_pConfig != nullptr)
    {
        utilities::IConfig::Destroy(m_pConfig);
        m_pConfig = nullptr;
    }
}

int32_t NetEngineImpl::Start()
{
    if (m_bRunning)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidCall, "{} is already started", m_strNetEngineName.c_str());
        return ErrorCode::kInvalidCall;
    }

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "start net engine: {}", m_strNetEngineName.c_str());

    try
    {
        m_bRunning = true;
        m_thManager = std::thread(&NetEngineImpl::ManagerWorker, this);
        for (auto &thIO : m_vecThIO)
        {
            thIO = std::thread(&NetEngineImpl::IOWorker, this);
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kThrowException, "falied to start net engine");
        return ErrorCode::kThrowException;
    }

    return ErrorCode::kSuccess;
}

void NetEngineImpl::Stop()
{
    if (!m_bRunning)
    {
        LOG_WARN(m_pLogger, ErrorCode::kInvalidCall, "{} is not started", m_strNetEngineName.c_str());
        return;
    }

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "stop net engine: {}", m_strNetEngineName.c_str());

    m_bRunning = false;
    m_thManager.join();
    for (auto &thIO : m_vecThIO)
    {
        thIO.join();
    }
}

ListenerHandler NetEngineImpl::CreateListener(utilities::IConfig *pConfig, ICallback *pCallback)
{
    ListenerHandler listenerHandler = {0, nullptr};
    if (pConfig == nullptr || pCallback == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid parameters");
        return listenerHandler;
    }

    std::unique_ptr<ListenerImpl> upListener(new(std::nothrow) ListenerImpl(m_pLogger));
    if (upListener == nullptr || upListener->Init(pConfig, pCallback) != ErrorCode::kSuccess)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kNoMemory, "Failed to create listener");
        return listenerHandler;
    }

    try
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_umapListener[listenerHandler.uID] = upListener.get();
        listenerHandler.uID = m_uNextListenerID++;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kThrowException, "Failed to create listener");
        return listenerHandler;
    }

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Create listener name: {}, id: {}", 
        upListener->GetName().c_str(), Wrap(listenerHandler.uID));
    
    listenerHandler.pHandler = upListener.release();
    return listenerHandler;
}

void NetEngineImpl::DestroyListener(ListenerHandler *pListenerHandler)
{
    if (pListenerHandler == nullptr || pListenerHandler->pHandler == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid listener handler");
        return;
    }

    auto it = m_umapListener.end();
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        it = m_umapListener.find(pListenerHandler->uID);
        if (it == m_umapListener.end())
        {
            LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Listener not found");
            return;
        }
        m_umapListener.erase(it);
    }

    ListenerImpl *pListener = it->second;

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Destroy listener name: {}, id: {}", 
        pListener->GetName().c_str(), Wrap(pListenerHandler->uID));

    pListener->Exit();
    pListenerHandler->uID = 0;
    pListenerHandler->pHandler = nullptr;
}
ConnectionHandler NetEngineImpl::CreateConnection(utilities::IConfig *pConfig, ICallback *pCallback)
{
    ConnectionHandler connectionHandler = {0, nullptr};
    if (pConfig == nullptr || pCallback == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid parameters");
        return connectionHandler;
    }

    std::unique_ptr<ConnectionImpl> upConnection(new(std::nothrow) ConnectionImpl(m_pLogger));
    if (upConnection == nullptr || upConnection->Init(pConfig, pCallback) != ErrorCode::kSuccess)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kNoMemory, "Failed to create connection");
        return connectionHandler;
    }

    try
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_umapConnection[connectionHandler.uID] = upConnection.get();
        connectionHandler.uID = m_uNextConnectionID++;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kThrowException, "Failed to create connection");
        return connectionHandler;
    }

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Create connection name: {}, id: {}", 
        upConnection->GetName().c_str(), Wrap(connectionHandler.uID));

    connectionHandler.pHandler = upConnection.release();
    return connectionHandler;
}

void NetEngineImpl::DestroyConnection(ConnectionHandler *pConnHandler)
{
    if (pConnHandler == nullptr || pConnHandler->pHandler == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid connection handler");
        return;
    }

    auto it = m_umapConnection.end();
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        it = m_umapConnection.find(pConnHandler->uID);
        if (it == m_umapConnection.end())
        {
            LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Connection not found");
            return;
        }
        m_umapConnection.erase(it);
    }

    ConnectionImpl *pConnection = it->second;

    LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Destroy connection name: {}, id: {}", 
        pConnection->GetName().c_str(), Wrap(pConnHandler->uID));

    pConnection->Exit();
    pConnHandler->uID = 0;
    pConnHandler->pHandler = nullptr;
}

int32_t NetEngineImpl::GetStats(std::string &strStats) const
{
    strStats.clear();
    return ErrorCode::kSuccess;
}

void NetEngineImpl::ManagerWorker()
{
    while (m_bRunning)
    {
        // 1. 处理网络事件
        // 2. 发送网络事件
    }
}

void NetEngineImpl::IOWorker()
{
    while (m_bRunning)
    {
        // 1. 接受网络事件
        // 2. 处理网络事件
        // 3. 发送网络事件
    }
}

}
}
