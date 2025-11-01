#include "memory_impl.h"
#include <error_code.h>
#include <new>

namespace lite_drive
{
namespace utilities
{

IMemory* IMemory::Create()
{
    return new(std::nothrow) MemoryImpl();
}

void IMemory::Destroy(IMemory *pMemory)
{
    if (pMemory != nullptr)
    {
        delete pMemory;
    }
}

int32_t MemoryImpl::Init(IConfig *pConfig)
{
    if (pConfig == nullptr)
    {
        return ErrorCode::kInvalidParam;
    }
    
    m_uMaxMemorySize = pConfig->GetInt64(config::kSection, config::kMaxMemoryMB, 0);
    m_uUsedMemorySize = 0;
    return ErrorCode::kSuccess;
}

void MemoryImpl::Exit()
{
    m_uMaxMemorySize = 0;
    m_uUsedMemorySize = 0;
}

void *MemoryImpl::New(uint32_t uSize)
{
    return new(std::nothrow) uint8_t[uSize];
}

void MemoryImpl::Delete(void *pMem)
{
    if (pMem != nullptr)
    {
        delete[] static_cast<uint8_t *>(pMem);
    }
}

int32_t MemoryImpl::GetStats(std::string &strStats) const
{
    strStats.clear();
    return ErrorCode::kSuccess;
}

}
}
