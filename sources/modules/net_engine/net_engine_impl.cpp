#include "net_engine_impl.h"
#include "config.h"
#include <error_code.h>
#include <new>

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
        m_strNetEngineName = m_pConfig->GetStr(config::kSection, config::kNetEngineName, "anonymous_net_engine");
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kThrowException, "Failed to get net engine name");
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
    if (m_pConfig != nullptr)
    {
        utilities::IConfig::Destroy(m_pConfig);
        m_pConfig = nullptr;
    }
}

int32_t NetEngineImpl::Start()
{
    return ErrorCode::kSuccess;
}

void NetEngineImpl::Stop()
{
}

ListenerHandler NetEngineImpl::CreateListener(utilities::IConfig *pConfig, ICallback *pCallback)
{
    ListenerHandler listenerHandler = {0, nullptr};
    if (pConfig == nullptr || pCallback == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid parameters");
        return listenerHandler;
    }

    return listenerHandler;
}

void NetEngineImpl::DestroyListener(ListenerHandler *pListenerHandler)
{
    if (pListenerHandler != nullptr)
    {
        LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Destroy listener handler");
    }
}
ConnectionHandler NetEngineImpl::CreateConnection(utilities::IConfig *pConfig, ICallback *pCallback)
{
    ConnectionHandler connectionHandler = {0, nullptr};
    if (pConfig == nullptr || pCallback == nullptr)
    {
        LOG_ERROR(m_pLogger, ErrorCode::kInvalidParam, "Invalid parameters");
        return connectionHandler;
    }

    return connectionHandler;
}

void NetEngineImpl::DestroyConnection(ConnectionHandler *pConnHandler)
{
    if (pConnHandler != nullptr)
    {
        LOG_EVENT(m_pLogger, ErrorCode::kEvent, "Destroy connection handler");
    }
}

int32_t NetEngineImpl::GetStats(std::string &strStats) const
{
    strStats.clear();
    return ErrorCode::kSuccess;
}

}
}
