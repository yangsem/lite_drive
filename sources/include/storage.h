#ifndef __LITE_DRIVE_STORAGE_H__
#define __LITE_DRIVE_STORAGE_H__

#include "config.h"
#include <vector>

namespace lite_drive
{
namespace storage
{

constexpr uint32_t kMaxFileNameLength = 256; // 文件名最大长度
constexpr uint32_t kMaxFilePathLength = 4096; // 文件路径最大长度

struct FileInfo
{
    uint64_t uID;                    // 文件ID, 最高位为1表示目录, 否则表示文件，其余位为文件ID
    uint64_t uParentID;              // 父文件ID，0表示根目录
    uint64_t uVersion;               // 文件版本，从1开始，每次修改版本号加1, 0表示未知
    uint64_t uSize;                  // 文件大小, 单位: 字节, 0表示未知
    uint32_t uCreateTime;            // 创建时间，YYYYMMDD-HHMMSS.ms格式, 0表示未知
    uint32_t uModifyTime;            // 修改时间，YYYYMMDD-HHMMSS.ms格式, 0表示未知
    char szName[kMaxFileNameLength]; // 文件名
};

enum class SeekMode
{
    kSet = 0, // 设置文件指针
    kCurrent = 1, // 当前位置
    kEnd = 2, // 文件末尾
};

class IFile
{
protected:
    virtual ~IFile() = default;

public:
    /**
     * @brief 读取文件
     * @param uOffset 偏移量
     * @param pData 数据
     * @param uLength 长度
     * @return 0表示成功,否则失败
     */
    virtual int32_t Read(uint64_t uOffset, uint8_t *pData, uint64_t uLength) = 0;

    /**
     * @brief 写入文件
     * @param uOffset 偏移量
     * @param pData 数据
     * @param uLength 长度
     * @return 0表示成功,否则失败
     */
    virtual int32_t Write(uint64_t uOffset, const uint8_t *pData, uint64_t uLength) = 0;

    /**
     * @brief 移动文件指针
     * @param iOffset 偏移量，单位: 字节
     * @param eSeekMode 移动模式
     * @return 0表示成功,否则失败
     */
    virtual int32_t Seek(int64_t iOffset, SeekMode eSeekMode) = 0;

    /**
     * @brief 获取文件指针
     * @return 文件指针，单位: 字节
     */
    virtual uint64_t Tell() = 0;

    /**
     * @brief 获取文件大小
     * @return 文件大小，单位: 字节
     */
    virtual uint64_t Size() = 0;

    /**
     * @brief 截断文件
     * @param uSize 文件大小，单位: 字节
     * @return 0表示成功,否则失败
     */
    virtual int32_t Truncate(uint64_t uSize) = 0;
};

template<typename T>
struct Handler
{
    uint64_t uID; // 句柄ID
    T *pHandler;     // 句柄数据
};

using FileHandler = Handler<IFile>;

class IStorage
{
protected:
    virtual ~IStorage() = default;
    
public:
    /**
     * @brief 创建存储对象
     * @return 存储对象指针,失败返回NULL
     */
    static IStorage* Create();

    /**
     * @brief 初始化存储对象
     * @param pConfig 配置
     * @return 0表示成功,否则失败
     */
    virtual int32_t Init(utilities::IConfig *pConfig) = 0;

    /**
     * @brief 退出存储对象
     */
    virtual void Exit() = 0;

    /**
     * @brief 设置当前用户，后续所有操作都基于当前用户进行
     * @param pUserName 用户名，为空表示清除当前用户
     * @return 0表示成功,否则失败
     * @note 允许递归操作
     */
    virtual int32_t SetCurrentUser(const char *pUserName) = 0;

    /**
     * @brief 读取目录
     * @param pDirPath 目录路径
     * @param vFileInfo 文件信息列表
     * @return 0表示成功,否则失败
     */
    virtual int32_t ReadDir(const char *pDirPath, std::vector<FileInfo> &vFileInfo) = 0;

    /**
     * @brief 获取文件信息
     * @param pPath 路径
     * @param sFileInfo 文件信息
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetFileInfo(const char *pPath, FileInfo &sFileInfo) = 0;

    /**
     * @brief 创建目录
     * @param pDirPath 目录路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t CreateDir(const char *pDirPath) = 0;

    /**
     * @brief 删除目录
     * @param pDirPath 目录路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t RemoveDir(const char *pDirPath) = 0;

    /**
     * @brief 复制文件(夹)
     * @param pSrcPath 源路径
     * @param pDstPath 目标路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t Copy(const char *pSrcPath, const char *pDstPath) = 0;

    /**
     * @brief 移动文件(夹)，重命名文件(夹)
     * @param pSrcPath 源路径
     * @param pDstPath 目标路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t Move(const char *pSrcPath, const char *pDstPath) = 0;

    /**
     * @brief 创建文件
     * @param pPath 路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t CreateFile(const char *pPath) = 0;

    /**
     * @brief 删除文件
     * @param pPath 路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t DeleteFile(const char *pPath) = 0;

    /**
     * @brief 打开文件
     * @param pPath 路径
     * @return 文件句柄,失败返回NULL
     */
    virtual FileHandler OpenFile(const char *pPath) = 0;

    /**
     * @brief 关闭文件
     * @param pFileHandler 文件句柄
     */
    virtual void CloseFile(FileHandler *pFileHandler) = 0;
};

namespace config
{

/* ============================== 存储配置 ============================== */
constexpr const char *kSection = "storage";       // 配置文件中的节名，类型: string
constexpr const char *kStoragePath = "storage_path"; // 存储路径，类型: string
constexpr const char *kIsCrypt = "is_crypt";           // 是否加密，类型: bool

}

}
}

#endif // __LITE_DRIVE_STORAGE_H__
