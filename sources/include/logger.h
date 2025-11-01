#ifndef __LITE_DRIVE_LOGGER_H__
#define __LITE_DRIVE_LOGGER_H__

#include "config.h"
#include <cstdint>
#include <cstdio>
#include <cinttypes>
#include <type_traits>

namespace lite_drive
{
namespace logger
{

enum class LogLevel
{
    kTrace = 0,   // 跟踪
    kDebug = 1,   // 调试
    kInfo = 2,    // 信息
    kWarn = 3,    // 警告
    kError = 4,   // 错误
    kFatal = 5,   // 致命错误
    kEvent = 6,   // 事件
};

class ILogger
{
protected:
    LogLevel m_eLogLevel; // 日志级别

    virtual ~ILogger() = default;

public:
    /**
     * @brief 创建日志器
     * @return 日志器指针,失败返回NULL
     */
    static ILogger *Create();

    /**
     * @brief 初始化日志器
     * @param pConfig 配置
     * @return 0表示成功,否则失败
     */
    virtual int32_t Init(IConfig *pConfig) = 0;

    /**
     * @brief 退出日志器
     */
    virtual void Exit() = 0;

    /**
     * @brief 启动日志器线程
     * @return 0表示成功,否则失败
     */
    virtual int32_t Start() = 0;

    /**
     * @brief 停止日志器线程
     */
    virtual void Stop() = 0;

    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void SetLogLevel(LogLevel level) { m_eLogLevel = level; }

    /**
     * @brief 获取日志级别
     * @return 日志级别
     */
    LogLevel GetLogLevel() const { return m_eLogLevel; }

    /**
     * @brief 获取日志器统计信息
     * @return 统计信息
     */
    virtual int32_t GetStats(std::string &strStats) const = 0;

    /**
     * @brief 记录日志
     * @param iErrorNo 错误编号
     * @param eLevel 日志级别
     * @param fmt 格式化字符串, 格式："this is a test {0} {1} {2}"
     * @param ppParams 参数数组, 格式：{"test", "test2", "test3"}
     * @param uParamCount 参数数量
     */
    virtual void Log(int32_t iErrorNo, LogLevel eLevel, const char *fmt, const char **ppParams, uint32_t uParamCount) = 0;
};

namespace wrap_detail
{

/* ============================== 辅助模板 ============================== */
template<typename T>
struct FormatHelper;

template<> struct FormatHelper<uint8_t>  { static const char* GetFormat() { return "%u"; } };
template<> struct FormatHelper<uint16_t> { static const char* GetFormat() { return "%u"; } };
template<> struct FormatHelper<uint32_t> { static const char* GetFormat() { return "%u"; } };
template<> struct FormatHelper<uint64_t> { static const char* GetFormat() { return "%" PRIu64; } };
template<> struct FormatHelper<int8_t>   { static const char* GetFormat() { return "%d"; } };
template<> struct FormatHelper<int16_t>  { static const char* GetFormat() { return "%d"; } };
template<> struct FormatHelper<int32_t>  { static const char* GetFormat() { return "%d"; } };
template<> struct FormatHelper<int64_t>  { static const char* GetFormat() { return "%" PRId64; } };
template<> struct FormatHelper<float>    { static const char* GetFormat() { return "%f"; } };
template<> struct FormatHelper<double>   { static const char* GetFormat() { return "%f"; } };

// bool类型的特殊处理
template<typename T>
inline void FormatValue(char* buffer, uint32_t size, const T& value, 
                        typename std::enable_if<!std::is_same<T, bool>::value>::type* = 0)
{
    snprintf(buffer, size, FormatHelper<T>::GetFormat(), value);
}

template<typename T>
inline void FormatValue(char* buffer, uint32_t size, const T& value,
                        typename std::enable_if<std::is_same<T, bool>::value>::type* = 0)
{
    snprintf(buffer, size, "%s", value ? "true" : "false");
}

// 根据类型获取推荐的缓冲区大小
template<typename T>
struct SizeHelper;

template<> struct SizeHelper<uint8_t>  { enum { value = 8 }; };
template<> struct SizeHelper<uint16_t> { enum { value = 8 }; };
template<> struct SizeHelper<uint32_t> { enum { value = 16 }; };
template<> struct SizeHelper<uint64_t> { enum { value = 32 }; };
template<> struct SizeHelper<int8_t>   { enum { value = 8 }; };
template<> struct SizeHelper<int16_t>  { enum { value = 8 }; };
template<> struct SizeHelper<int32_t>  { enum { value = 16 }; };
template<> struct SizeHelper<int64_t>  { enum { value = 32 }; };
template<> struct SizeHelper<float>    { enum { value = 32 }; };
template<> struct SizeHelper<double>   { enum { value = 32 }; };
template<> struct SizeHelper<bool>     { enum { value = 8 }; };

}

template<uint32_t Size, typename T>
class Wrap
{
public:
    Wrap(const T &value)
    {
        wrap_detail::FormatValue(m_szBuffer, Size, value);
    }

    const char * operator()() const { return m_szBuffer; }

private:
    char m_szBuffer[Size];
};

using WrapU8 = Wrap<8, uint8_t>;
using WrapU16 = Wrap<16, uint16_t>;
using WrapU32 = Wrap<32, uint32_t>;
using WrapU64 = Wrap<64, uint64_t>;
using WrapI8 = Wrap<8, int8_t>;
using WrapI16 = Wrap<16, int16_t>;
using WrapI32 = Wrap<32, int32_t>;
using WrapI64 = Wrap<64, int64_t>;
using WrapF = Wrap<32, float>;
using WrapD = Wrap<32, double>;
using WrapB = Wrap<8, bool>;

// 自动类型推断宏：根据参数类型自动选择对应的Wrap实现
// 使用方式：Wrap(a) 会自动推断类型并返回对应的Wrap实例
#define Wrap(value)                                             \
    (::lite_drive::logger::Wrap<                                \
        ::lite_drive::logger::wrap_detail::SizeHelper<          \
            typename std::decay<decltype(value)>::type>::value, \
        typename std::decay<decltype(value)>::type>(value))

#define LOG_BASE(logger, eLevel, iErrorNo, fmt, ...)                               \
{                                                                                  \
char pParams[] = {__VA_ARGS__};                                                    \
logger->Log(iErrorNo, eLevel, fmt, pParams, sizeof(pParams) / sizeof(pParams[0])); \
}

/* ============================== 日志宏 ============================== */
#define LOG_TRACE(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kTrace, iErrorNo, fmt, __VA_ARGS__) // 跟踪
#define LOG_DEBUG(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kDebug, iErrorNo, fmt, __VA_ARGS__) // 调试
#define LOG_INFO(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kInfo, iErrorNo, fmt, __VA_ARGS__)   // 信息
#define LOG_WARN(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kWarn, iErrorNo, fmt, __VA_ARGS__)   // 警告
#define LOG_ERROR(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kError, iErrorNo, fmt, __VA_ARGS__) // 错误
#define LOG_FATAL(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kFatal, iErrorNo, fmt, __VA_ARGS__) // 致命错误
#define LOG_EVENT(logger, iErrorNo, fmt, ...) LOG_BASE(logger, LogLevel::kEvent, iErrorNo, fmt, __VA_ARGS__) // 事件

namespace config
{

/* ============================== 日志器配置 ============================== */
constexpr const char *kSection = "logger";                   // 配置文件中的节名，类型: string
constexpr const char *kLogLevel = "log_level";               // 日志级别，类型: enum
constexpr const char *kLogAsync = "log_async";               // 是否异步，类型: bool
constexpr const char *kLogPath = "log_path";                 // 日志路径，类型: string
constexpr const char *kLogName = "log_name";                 // 日志文件前缀名，类型: string
constexpr const char *kLogFileMaxSize = "log_file_max_size"; // 日志文件最大大小(MB)，类型: uint32_t
constexpr const char *kLogFileMaxFile = "log_file_max_file"; // 日志文件最大数量，类型: uint32_t

}

}
}

#endif // __LITE_DRIVE_LOGGER_H__
