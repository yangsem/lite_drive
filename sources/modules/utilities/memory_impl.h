#ifndef __LITE_DRIVE_MEMORY_IMPL_H__
#define __LITE_DRIVE_MEMORY_IMPL_H__

#include <memory.h>

namespace lite_drive
{
namespace utilities
{
class MemoryImpl : public IMemory
{
public:
    MemoryImpl() = default;
    ~MemoryImpl() override = default;

    int32_t Init(IConfig *pConfig) override;
    void Exit() override;
    void *New(uint32_t uSize) override;
    void Delete(void *pMem) override;
    int32_t GetStats(std::string &strStats) const override;

private:
    uint64_t m_uMaxMemorySize{0};
    uint64_t m_uUsedMemorySize{0};
};

}
}
#endif // __LITE_DRIVE_MEMORY_IMPL_H__
