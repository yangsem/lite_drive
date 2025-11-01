#ifndef __LITE_DRIVE_NET_ENGINE_IMPL_H__
#define __LITE_DRIVE_NET_ENGINE_IMPL_H__

#include <net_engine.h>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include "listener_impl.h"
#include "connection_impl.h"

namespace lite_drive
{
namespace net_engine
{

class NetEngineImpl : public INetEngine
{
public:
    NetEngineImpl(logger::ILogger *pLogger);
    ~NetEngineImpl() override;

    int32_t Init(utilities::IConfig *pConfig, ICallback *pGlobalCallback) override;
    void Exit() override;
    int32_t Start() override;
    void Stop() override;
    ListenerHandler CreateListener(utilities::IConfig *pConfig, ICallback *pCallback) override;
    void DestroyListener(ListenerHandler *pListenerHandler) override;
    ConnectionHandler CreateConnection(utilities::IConfig *pConfig, ICallback *pCallback) override;
    void DestroyConnection(ConnectionHandler *pConnHandler) override;
    int32_t GetStats(std::string &strStats) const override;

private:
    void IOWorker();
    void ManagerWorker();

private:
    volatile bool m_bRunning{false};
    std::thread m_thManager;
    std::vector<std::thread> m_vecThIO;

    std::mutex m_mutex;
    std::atomic<uint64_t> m_uNextListenerID{1};
    std::unordered_map<uint64_t, ListenerImpl *> m_umapListener;
    std::atomic<uint64_t> m_uNextConnectionID{1};
    std::unordered_map<uint64_t, ConnectionImpl *> m_umapConnection;

    logger::ILogger *m_pLogger{nullptr};
    utilities::IConfig *m_pConfig{nullptr};
    ICallback *m_pGlobalCallback{nullptr};
    std::string m_strNetEngineName;
};

}
}
#endif // __LITE_DRIVE_NET_ENGINE_IMPL_H__
