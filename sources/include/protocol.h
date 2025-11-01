#ifndef __LITE_DRIVE_PROTOCOL_H__
#define __LITE_DRIVE_PROTOCOL_H__

#include <cstdint>

namespace lite_drive
{
namespace protocol
{

constexpr const char *kMagic = "LIDR"; // 魔数

enum class Version : uint8_t
{
    kVersion1 = 1, // 版本1
    kMax,
};

enum class Method : uint16_t
{
    kLogin = 1000, // 登录
    kSignUp,
    kQuerySignUp,
    kProcessSignUp,
    kQuerySignUpResult,
    kModifyPassword,
    kCreateTag,
    kQueryTags,
    kDeleteTag,
    kUpdataUserTag,

    kReadDir = 2000,
    kGetFileInfo,
    kCreateDir,
    kRemoveDir,
    kCopy,
    kMove,
    kCreateFile,
    kDeleteFile,
    kOpenFile,

    kShareTo = 3000,
    kUpdateShare,
    kCancelShare,
    kQueryShareMe,
    kQueryShareTo,

    kCreateZone = 4000,
    kQueryZone,
    kDeleteZone,
    kMax,
};

struct ProtocolHeader
{
    char szMagic[4];       // 魔数
    uint32_t uTotalLength; // 总长度,[1, UINT32_MAX - 1]
    uint8_t uHeaderLength; // 头部长度
    Version eVersion;      // 版本
    uint16_t uSequence;    // 序列号
    Method eMethod;        // 方法
    uint8_t uOptionBegin;  // 可选头的起始位置
    uint8_t uFlags;        // 标志
    uint8_t uOption[0];    // 选项
};

}
}

#endif // __LITE_DRIVE_PROTOCOL_H__
