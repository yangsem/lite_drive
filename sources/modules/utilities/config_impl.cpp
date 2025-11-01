#include "config_impl.h"
#include <error_code.h>
#include <fstream>
#include <json/reader.h>
#include <json/writer.h>
#include <new>

namespace lite_drive
{
namespace utilities
{

IConfig* IConfig::Create()
{
    return new(std::nothrow) ConfigImpl();
}

void IConfig::Destroy(IConfig *pConfig)
{
    if (pConfig != nullptr)
    {
        delete pConfig;
    }
}

int32_t ConfigImpl::Load(const char* pConfigFile)
{
    if (pConfigFile == nullptr)
    {
        return ErrorCode::kInvalidParam;
    }

    try
    {
        m_configFile = pConfigFile; 
        std::ifstream ifs(pConfigFile);
        if (!ifs.is_open())
        {
            return ErrorCode::kFileNotFound;
        }

        Json::CharReaderBuilder builder;
        std::string errs;
        if (!parseFromStream(builder, ifs, &m_config, &errs))
        {
            return ErrorCode::kJsonParseFailed;
        }
    }
    catch(const std::exception& e)
    {
        return ErrorCode::kThrowException;
    }
    
    return ErrorCode::kSuccess;
}

int32_t ConfigImpl::Save(const char* pConfigFile) const
{
    if (pConfigFile == nullptr)
    {
        pConfigFile = m_configFile.c_str();
    }

    try
    {
        std::ofstream ofs(pConfigFile);
        if (!ofs.is_open())
        {
            return ErrorCode::kFileOpenFailed;
        }

        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(m_config, &ofs);
    }
    catch(const std::exception& e)
    {
        return ErrorCode::kThrowException;
    }
    return ErrorCode::kSuccess;
}

int32_t ConfigImpl::Copy(const IConfig *pConfig)
{
    auto pConfigImpl = dynamic_cast<const ConfigImpl *>(pConfig);
    if (pConfigImpl == nullptr)
    {
        return ErrorCode::kInvalidParam;
    }

    try
    {
        m_config = pConfigImpl->m_config;
    }
    catch(const std::exception& e)
    {
        return ErrorCode::kThrowException;
    }

    return ErrorCode::kSuccess;
}

std::string ConfigImpl::ToString() const
{
    try
    {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::stringstream ss;
        writer->write(m_config, &ss);
        return ss.str();
    }
    catch(const std::exception& e)
    {
    }
    return std::string();
}

template<typename T>
int32_t ConfigImpl::SetValue(const char *pSection, const char *pKey, T value)
{
    if (pSection == nullptr || pKey == nullptr)
    {
        return ErrorCode::kInvalidParam;
    }

    try
    {
        m_config[pSection][pKey] = value;
    }
    catch(const std::exception& e)
    {
        return ErrorCode::kThrowException;
    }
    return ErrorCode::kSuccess;
}

int32_t ConfigImpl::SetBool(const char *pSection, const char *pKey, bool bValue)
{
    return SetValue(pSection, pKey, bValue);
}

int32_t ConfigImpl::SetInt32(const char *pSection, const char *pKey, int32_t iValue)
{
    return SetValue(pSection, pKey, iValue);
}

int32_t ConfigImpl::SetInt64(const char *pSection, const char *pKey, int64_t iValue)
{
    return SetValue(pSection, pKey, iValue);
}

int32_t ConfigImpl::SetStr(const char *pSection, const char *pKey, const char *pValue)
{
    return SetValue(pSection, pKey, pValue);
}

template<typename T>
T ConfigImpl::GetValue(const char *pSection, const char *pKey, T defaultValue) const
{
    if (pSection == nullptr || pKey == nullptr)
    {
        return defaultValue;
    }
    
    try
    {
        return m_config[pSection][pKey].as<T>();
    }
    catch(const std::exception& e)
    {
        return defaultValue;
    }
    return defaultValue;
}

bool ConfigImpl::GetBool(const char *pSection, const char *pKey, bool bDefault) const
{
    return GetValue(pSection, pKey, bDefault);
}

int32_t ConfigImpl::GetInt32(const char *pSection, const char *pKey, int32_t iDefault) const
{
    return GetValue(pSection, pKey, iDefault);
}

int64_t ConfigImpl::GetInt64(const char *pSection, const char *pKey, int64_t iDefault) const
{
    return GetValue(pSection, pKey, iDefault);
}

const char *ConfigImpl::GetStr(const char *pSection, const char *pKey, const char *pDefault) const
{
    return GetValue(pSection, pKey, pDefault);
}

}
}