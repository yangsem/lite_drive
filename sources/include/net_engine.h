#ifndef __LITE_DRIVE_NET_ENGINE_H__
#define __LITE_DRIVE_NET_ENGINE_H__

#include "config.h"
#include "logger.h"
#include <string>

namespace lite_drive
{
namespace net_engine
{

constexpr const char *kModuleName = "net_engine"; // 模块名称

struct IMessage
{
public:
    uint8_t *pData;   // 消息数据
    uint32_t uLength; // 消息长度
};

class IConnection
{
protected:
    virtual ~IConnection() = default;

public:
    /**
     * @brief 创建消息
     * @param uLength 消息长度
     * @return 消息指针,失败返回NULL
     */
    virtual IMessage *NewMessage(uint32_t uLength) = 0;

    /**
     * @brief 释放消息
     * @param pMessage 消息指针
     */
    virtual void DeleteMessage(IMessage *pMessage) = 0;
    
    /**
     * @brief 零拷贝发送消息
     * @param pMessage 消息指针
     * @return 0表示成功,否则失败
     */
    virtual int32_t SendMessage(IMessage *pMessage) = 0;

    /**
     * @brief 非零拷贝发送消息
     * @param pData 消息数据
     * @param uLength 消息长度
     * @return 0表示成功,否则失败
     */
    virtual int32_t SendMessage(const uint8_t *pData, uint32_t uLength) = 0;

    /**
     * @brief 零拷贝同步调用
     * @param pRequest 请求消息指针
     * @param pResponse 响应消息指针
     * @return 0表示成功,否则失败
     */
    virtual int32_t Call(IMessage *pRequest, IMessage *pResponse) = 0;

    /**
     * @brief 非零拷贝同步调用
     * @param pRequest 请求消息数据
     * @param uRequestLength 请求消息长度
     * @param pResponse 响应消息指针
     * @return 0表示成功,否则失败
     */
    virtual int32_t Call(const uint8_t *pRequest, uint32_t uRequestLength, IMessage *pResponse) = 0;
    
    /**
     * @brief 连接远程服务器
     * @param pRemoteIP 远程服务器IP地址
     * @param iRemotePort 远程服务器端口
     * @return 0表示成功,否则失败
     */
    virtual int32_t Connect(const char *pRemoteIP, uint16_t iRemotePort) = 0;

    /**
     * @brief 关闭连接
     */
    virtual void Close() = 0;

    /**
     * @brief 是否连接成功
     * @return 是否连接成功
     */
    virtual bool IsConnected() const = 0;

    /**
     * @brief 获取远程服务器IP地址
     * @return 远程服务器IP地址
     */
    virtual const char *GetRemoteIP() const = 0;

    /**
     * @brief 获取远程服务器端口
     * @return 远程服务器端口
     */
    virtual uint16_t GetRemotePort() const = 0;

    /**
     * @brief 获取本地IP地址
     * @return 本地IP地址
     */
    virtual const char *GetLocalIP() const = 0;

    /**
     * @brief 获取本地端口
     * @return 本地端口
     */
    virtual uint16_t GetLocalPort() const = 0;
};

class IListener
{
protected:
    virtual ~IListener() = default;

public:
    
};

template<typename T>
struct Handler
{
    uint64_t uID; // 句柄ID
    T *pHandler; // 句柄数据
};

using ConnectionHandler = Handler<IConnection>;
using ListenerHandler = Handler<IListener>;

class ICallback
{
protected:
    virtual ~ICallback() = default;
    
public:
    /**
     * @brief 获取消息长度
     * @param pConnHandler 连接句柄
     * @param pData 消息数据
     * @param uLength 消息长度
     * @return 完整消息长度，0表示无法识别，UINT32_MAX表示异常数据断开连接
     */
    virtual uint32_t OnMessageLength(ConnectionHandler *pConnHandler, const uint8_t *pData, uint32_t uLength) = 0;

    /**
     * @brief 处理消息
     * @param pConnHandler 连接句柄
     * @param pData 消息数据
     * @param uLength 消息长度
     * @return 处理结果，0表示成功，否则表示异常断开连接
     */
    virtual int32_t OnMessage(ConnectionHandler *pConnHandler, const uint8_t *pData, uint32_t uLength) = 0;

    /**
     * @brief 事件回调
     * @param pConnHandler 连接句柄
     * @param pEventMsg 事件消息
     */
    virtual void OnEvent(ConnectionHandler *pConnHandler, const char *pEventMsg) = 0;

    /**
     * @brief 连接成功回调
     * @param pConnHandler 连接句柄
     */
    virtual void OnConnected(ConnectionHandler *pConnHandler) = 0;

    /**
     * @brief 连接断开回调
     * @param pConnHandler 连接句柄
     */
    virtual void OnDisconnected(ConnectionHandler *pConnHandler) = 0;
};

enum class NetEngineType
{
    kTcp, // TCP协议
    kUdp, // UDP协议
    kP2P, // P2P协议
};

class INetEngine
{
protected:
    virtual ~INetEngine() = default;
    
public:
    /**
     * @brief 创建网络引擎
     * @param pLogger 日志器
     * @return 网络引擎指针,失败返回NULL
     */
    static INetEngine* Create(logger::ILogger *pLogger);

    /**
     * @brief 销毁网络引擎
     * @param pNetEngine 网络引擎指针
     */
    static void Destroy(INetEngine *pNetEngine);

    /**
     * @brief 初始化网络引擎
     * @param pConfig 配置
     * @param pGlobalCallback 全局回调
     * @return 0表示成功,否则失败
     */
    virtual int32_t Init(utilities::IConfig *pConfig, ICallback *pGlobalCallback) = 0;

    /**
     * @brief 退出网络引擎
     */
    virtual void Exit() = 0;

    /**
     * @brief 启动网络引擎线程
     * @return 0表示成功,否则失败
     */
    virtual int32_t Start() = 0;

    /**
     * @brief 停止网络引擎线程
     */
    virtual void Stop() = 0;

    /**
     * @brief 创建监听器
     * @param pConfig 配置
     * @param pCallback 回调
     * @return 监听器句柄,失败返回NULL
     */
    virtual ListenerHandler CreateListener(utilities::IConfig *pConfig, ICallback *pCallback) = 0;

    /**
     * @brief 销毁监听器
     * @param pListenerHandler 监听器句柄
     */
    virtual void DestroyListener(ListenerHandler *pListenerHandler) = 0;

    /**
     * @brief 创建连接
     * @param pConfig 配置
     * @param pCallback 回调
     * @return 连接句柄,失败返回NULL
     */
    virtual ConnectionHandler CreateConnection(utilities::IConfig *pConfig, ICallback *pCallback) = 0;

    /**
     * @brief 销毁连接
     * @param pConnHandler 连接句柄
     */
    virtual void DestroyConnection(ConnectionHandler *pConnHandler) = 0;

    /**
     * @brief 获取网络引擎统计信息
     * @param strStats 统计信息
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetStats(std::string &strStats) const = 0;
};

namespace config
{
/* ============================== 网络引擎配置 ============================== */
constexpr const char *kSection = "net_engine";            // 配置文件中的节名，类型: string
constexpr const char *kNetEngineType = "net_engine_type"; // 网络引擎类型，类型: string
constexpr const char *kNetEngineName = "net_engine_name"; // 网络引擎名称，类型: string
constexpr const char *kIOThreadCount = "io_thread_count"; // IO线程数量，类型: uint32_t

/* ============================== 网络引擎Listener配置 ============================== */
constexpr const char *kListenerName = "listener_name"; // 监听器名称，类型: string
constexpr const char *kListenerIP = "listener_ip";     // 网络引擎IP地址，类型: string
constexpr const char *kListenerPort = "listener_port"; // 网络引擎端口，类型: uint16_t

/* ============================== 网络引擎Connection配置 ============================== */
constexpr const char *kConnectionName = "connection_name";              // 连接名称，类型: string
constexpr const char *kConnectionRemoteIP = "connection_remote_ip";     // 远程服务器IP地址，类型: string
constexpr const char *kConnectionRemotePort = "connection_remote_port"; // 远程服务器端口，类型: uint16_t

/* ============================== 网络引擎公共配置,全局和监听器、连接都可以使用 ============================== */
constexpr const char *kSocketBufferBytes = "socket_buffer_bytes";     // 套接字缓冲区字节大小，类型: uint32_t
constexpr const char *kHeartbeatIntervalMs = "heartbeat_interval_ms"; // 心跳间隔，类型: uint32_t
constexpr const char *kHeartbeatTimeoutMs = "heartbeat_timeout_ms";   // 心跳超时，类型: uint32_t
}

namespace default_value
{
/* ============================== 网络引擎公共默认值 ============================== */
constexpr const char *kNetEngineType = "tcp"; // 网络引擎类型，默认TCP
constexpr const char *kNetEngineName = "anonymous_net_engine"; // 网络引擎名称，默认匿名网络引擎
constexpr const uint32_t kIOThreadCount = 1; // IO线程数量，默认1

/* ============================== 网络引擎Listener默认值 ============================== */
constexpr const char *kListenerName = "anonymous_listener"; // 监听器名称，默认匿名监听器
constexpr const char *kListenerIP = "0.0.0.0"; // 监听器IP地址，默认所有IP
constexpr const uint32_t kListenerPort = 8080; // 监听器端口，默认8080

/* ============================== 网络引擎Connection默认值 ============================== */
constexpr const char *kConnectionName = "anonymous_connection"; // 连接名称，默认匿名连接
constexpr const char *kConnectionRemoteIP = "127.0.0.1"; // 远程服务器IP地址，默认127.0.0.1
constexpr const uint32_t kConnectionRemotePort = 8080; // 远程服务器端口，默认8080

/* ============================== 网络引擎公共默认值 ============================== */
constexpr const uint32_t kSocketBufferBytes = 0; // 套接字缓冲区字节大小，默认不设置,使用系统默认值
constexpr const uint32_t kHeartbeatIntervalMs = 1000; // 心跳间隔，默认1秒
constexpr const uint32_t kHeartbeatTimeoutMs = 30000; // 心跳超时，默认30秒
}

}
}

#endif // __LITE_DRIVE_NET_ENGINE_H__
