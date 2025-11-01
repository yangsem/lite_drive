#ifndef __LITE_DRIVE_ERROR_CODE_H__
#define __LITE_DRIVE_ERROR_CODE_H__

#include <cstdint>

namespace lite_drive
{

enum ErrorCode : int32_t
{
    // 通用错误
    kSuccess = 0,
    kEvent,
    kInvalidParam,
    kInvalidCall,
    kThrowException,
    kNoMemory,

    // json 相关错误
    kJsonParseFailed = 100,
    kJsonSaveFailed = 101,

    // 文件相关错误
    kFileNotFound = 1000,
    kFIleCreateFailed = 1001,
    kFileOpenFailed = 1002,
    kFileReadFailed = 1003,
    kFIleWriteFailed = 1004,

    // 目录相关错误
    kDirNotFound = 2000,
    kDirCreateFailed = 2001,
    kReadDirFailed = 2002,
};

}
#endif // __LITE_DRIVE_ERROR_CODE_H__