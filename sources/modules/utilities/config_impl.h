#ifndef __LITE_DRIVE_CONFIG_IMPL_H__
#define __LITE_DRIVE_CONFIG_IMPL_H__

#include <config.h>
#include <json/value.h>

namespace lite_drive
{
namespace utilities
{

class ConfigImpl : public IConfig
{
public:
    ConfigImpl() = default;
    ~ConfigImpl() override = default;

    int32_t Load(const char* pConfigFile) override;
    int32_t Save(const char* pConfigFile = nullptr) const override;
    std::string ToString() const override;
    int32_t Copy(const IConfig *pConfig) override;
    int32_t SetBool(const char *pSection, const char *pKey, bool bValue) override;
    int32_t SetInt32(const char *pSection, const char *pKey, int32_t iValue) override;
    int32_t SetInt64(const char *pSection, const char *pKey, int64_t iValue) override;
    int32_t SetStr(const char *pSection, const char *pKey, const char *pValue) override;
    bool GetBool(const char *pSection, const char *pKey, bool bDefault = false) const override;
    int32_t GetInt32(const char *pSection, const char *pKey, int32_t iDefault = 0) const override;
    int64_t GetInt64(const char *pSection, const char *pKey, int64_t iDefault = 0) const override;
    const char *GetStr(const char *pSection, const char *pKey, const char *pDefault = nullptr) const override;

private:
    template<typename T>
    int32_t SetValue(const char *pSection, const char *pKey, T value);

    template<typename T>
    T GetValue(const char *pSection, const char *pKey, T defaultValue) const;

private:
    Json::Value m_config;
    std::string m_configFile;
};

}
}
#endif // __LITE_DRIVE_CONFIG_IMPL_H__
