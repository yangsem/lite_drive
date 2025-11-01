#ifndef __LITE_DRIVE_NET_ENGINE_LISTENER_H__
#define __LITE_DRIVE_NET_ENGINE_LISTENER_H__

#include <net_engine.h>

namespace lite_drive
{
namespace net_engine
{

class ListenerImpl : public IListener
{
public:
    ListenerImpl(logger::ILogger *pLogger);
    ~ListenerImpl() override = default;

    int32_t Init(utilities::IConfig *pConfig, ICallback *pCallback);
    void Exit();

    int32_t Accept();

    std::string &GetName() const;

private:
    int32_t m_iEpollFd{-1};
    int32_t m_iListenFd{-1};
    ICallback *m_pCallback{nullptr};

    std::string m_strListenerName;
    std::string m_strListenerIP;
    uint16_t m_uListenerPort{0};
    
    logger::ILogger *m_pLogger{nullptr};
    utilities::IConfig *m_pConfig{nullptr};
};

}
}
#endif // __LITE_DRIVE_NET_ENGINE_LISTENER_H__
