#ifndef __LITE_DRIVE_MEMORY_H__
#define __LITE_DRIVE_MEMORY_H__

#include "config.h"
#include <string>

namespace lite_drive
{
namespace utilities
{

class IMemory
{
protected:
    virtual ~IMemory() = default;
    
public:
    /**
     * @brief 创建内存对象
     * @return 内存对象指针,失败返回NULL
     */
    static IMemory* Create();

    /**
     * @brief 销毁内存对象
     * @param pMemory 内存对象指针
     */
    static void Destroy(IMemory *pMemory);

    /**
     * @brief 初始化内存对象
     * @param pConfig 配置
     * @return 0表示成功,否则失败
     */
    virtual int32_t Init(IConfig *pConfig) = 0;
    
    
    /**
     * @brief 退出内存对象
     */
    virtual void Exit() = 0;

    /**
     * @brief 分配内存
     * @param uSize 内存大小
     * @return 内存指针,失败返回NULL
     */
    virtual void *New(uint32_t uSize) = 0;

    /**
     * @brief 释放内存
     * @param pMem 内存指针
     */
    virtual void Delete(void *pMem) = 0;

    /**
     * @brief 获取内存大小
     * @param pMem 内存指针
     * @return 内存大小
     */
    virtual int32_t GetStats(std::string &strStats) const = 0;
};

namespace config
{

/* ============================== 内存配置 ============================== */
constexpr const char *kSection = "memory"; // 配置文件中的节名，类型: string
constexpr const char *kMaxMemoryMB = "max_memory_mb"; // 最大内存大小，类型: uint32_t

}

}
}

#endif // __LITE_DRIVE_MEMORY_H__
