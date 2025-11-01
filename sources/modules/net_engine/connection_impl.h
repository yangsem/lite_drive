#ifndef __LITE_DRIVE_NET_ENGINE_CONNECTION_IMPL_H__
#define __LITE_DRIVE_NET_ENGINE_CONNECTION_IMPL_H__

#include <net_engine.h>

namespace lite_drive
{
namespace net_engine
{

class ConnectionImpl : public IConnection
{
public:
    ConnectionImpl(logger::ILogger *pLogger);
    ~ConnectionImpl() override = default;

    int32_t Init(utilities::IConfig *pConfig, ICallback *pCallback);
    void Exit();

    IMessage *NewMessage(uint32_t uLength) override;
    void DeleteMessage(IMessage *pMessage) override;
    int32_t SendMessage(IMessage *pMessage) override;
    int32_t SendMessage(const uint8_t *pData, uint32_t uLength) override;
    int32_t Call(IMessage *pRequest, IMessage *pResponse) override;
    int32_t Call(const uint8_t *pRequest, uint32_t uRequestLength, IMessage *pResponse) override;
    int32_t Connect(const char *pRemoteIP, uint16_t iRemotePort) override;
    void Close() override;
    bool IsConnected() const override;
    const char *GetRemoteIP() const override;
    uint16_t GetRemotePort() const override;
    const char *GetLocalIP() const override;
    uint16_t GetLocalPort() const override;

    std::string &GetName() const;

private:
    int32_t m_iFd{-1};
    ICallback *m_pCallback{nullptr};

    std::string m_strConnectionName;
    std::string m_strRemoteIP;
    uint16_t m_uRemotePort{0};
    std::string m_strLocalIP;
    uint16_t m_uLocalPort{0};

    logger::ILogger *m_pLogger{nullptr};
    utilities::IConfig *m_pConfig{nullptr};
};

}
}
#endif // __LITE_DRIVE_NET_ENGINE_CONNECTION_IMPL_H__
