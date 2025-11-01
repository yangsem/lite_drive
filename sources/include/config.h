#ifndef __LITE_DRIVE_CONFIG_H__
#define __LITE_DRIVE_CONFIG_H__

#include <cstdint>
#include <string>

namespace lite_drive
{

class IConfig
{
protected:
    virtual ~IConfig() = default;
    
public:
    /**
     * @brief 创建配置对象
     * @return 配置指针,失败返回NULL
     */
    static IConfig* Create();

    /**
     * @brief 加载配置文件
     * @param pConfigFile 配置文件路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t Load(const char* pConfigFile) = 0;

    /**
     * @brief 保存配置文件
     * @param pConfigFile 配置文件路径
     * @return 0表示成功,否则失败
     */
    virtual int32_t Save(const char* pConfigFile) = 0;

    /**
     * @brief 转换为字符串
     * @return 字符串
     */
    virtual std::string ToString() const = 0;

    /**
     * @brief 设置布尔值
     * @param pSection 节名
     * @param pKey 键名
     * @param bValue 值
     * @return 0表示成功,否则失败
     */
    virtual int32_t SetBool(const char *pSection, const char *pKey, bool bValue) = 0;

    /**
     * @brief 设置32位整数
     * @param pSection 节名
     * @param pKey 键名
     * @param iValue 值
     * @return 0表示成功,否则失败
     */
    virtual int32_t SetInt32(const char *pSection, const char *pKey, int32_t iValue) = 0;

    /**
     * @brief 设置64位整数
     * @param pSection 节名
     * @param pKey 键名
     * @param iValue 值
     * @return 0表示成功,否则失败
     */
    virtual int32_t SetInt64(const char *pSection, const char *pKey, int64_t iValue) = 0;

    /**
     * @brief 设置字符串
     * @param pSection 节名
     * @param pKey 键名
     * @param pValue 值
     * @return 0表示成功,否则失败
     */
    virtual int32_t SetStr(const char *pSection, const char *pKey, const char *pValue) = 0;

    /**
     * @brief 获取布尔值
     * @param pSection 节名
     * @param pKey 键名
     * @param bDefault 默认值
     * @return 值
     */
    virtual bool GetBool(const char *pSection, const char *pKey, bool bDefault = false) const = 0;

    /**
     * @brief 获取32位整数
     * @param pSection 节名
     * @param pKey 键名
     * @param iDefault 默认值
     * @return 值
     */
    virtual int32_t GetInt32(const char *pSection, const char *pKey, int32_t iDefault = 0) const = 0;

    /**
     * @brief 获取64位整数
     * @param pSection 节名
     * @param pKey 键名
     * @param iDefault 默认值
     * @return 值
     */
    virtual int64_t GetInt64(const char *pSection, const char *pKey, int64_t iDefault = 0) const = 0;

    /**
     * @brief 获取字符串
     * @param pSection 节名
     * @param pKey 键名
     * @param pDefault 默认值
     * @return 值
     */
    virtual const char *GetStr(const char *pSection, const char *pKey, const char *pDefault = nullptr) const = 0;
};

}

#endif // __LITE_DRIVE_CONFIG_H__
