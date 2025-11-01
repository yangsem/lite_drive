#ifndef __LITE_DRIVE_NET_ENGINE_IMPL_H__
#define __LITE_DRIVE_NET_ENGINE_IMPL_H__

#include "logger.h"
#include <net_engine.h>

namespace lite_drive
{
namespace net_engine
{

class NetEngineImpl : public INetEngine
{
public:
    NetEngineImpl(logger::ILogger *pLogger);
    ~NetEngineImpl() override = default;

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
    logger::ILogger *m_pLogger{nullptr};
    utilities::IConfig *m_pConfig{nullptr};
    ICallback *m_pGlobalCallback{nullptr};
    std::string m_strNetEngineName;
};

}
}
#endif // __LITE_DRIVE_NET_ENGINE_IMPL_H__
