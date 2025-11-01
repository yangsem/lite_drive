#ifndef __LITE_DRIVE_USER_MANAGER_H__
#define __LITE_DRIVE_USER_MANAGER_H__

#include "config.h"
#include <vector>
#include <string>

namespace lite_drive
{
namespace user_manager
{

struct UserInfo
{
    uint64_t uUserID;                // 用户ID
    std::vector<std::string> vTags; // 标签列表
    std::string strUserName;        // 用户名
    std::string strPassword;        // 密码
};

class IUserManager
{
protected:
    virtual ~IUserManager() = default;
    
public:
    /**
     * @brief 创建用户管理器
     * @return 用户管理器指针,失败返回NULL
     */
    static IUserManager* Create();

    /**
     * @brief 初始化用户管理器
     * @param pConfig 配置
     * @return 0表示成功,否则失败
     */
    virtual int32_t Init(IConfig *pConfig) = 0;

    /**
     * @brief 退出用户管理器
     */
    virtual void Exit() = 0;

    /**
     * @brief 添加用户
     * @param pUserName 用户名
     * @param pPassword 密码
     * @return 0表示成功,否则失败
     */
    virtual int32_t AddUser(const char *pUserName, const char *pPassword) = 0;

    /**
     * @brief 删除用户
     * @param pUserName 用户名
     * @return 0表示成功,否则失败
     */
    virtual int32_t RemoveUser(const char *pUserName) = 0;

    /**
     * @brief 获取用户列表
     * @param vUsers 用户列表
     * @param uOffset 偏移量
     * @param uLimit 限制数量
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetUsers(std::vector<UserInfo> &vUsers, uint32_t uOffset, uint32_t uLimit) const = 0;

    /**
     * @brief 用户认证
     * @param pUserName 用户名
     * @param pPassword 密码
     * @return 0表示成功,否则失败
     */
    virtual int32_t Login(const char *pUserName, const char *pPassword) = 0;

    /**
     * @brief 用户注销
     * @param pUserName 用户名
     * @return 0表示成功,否则失败
     */
    virtual int32_t Logout(const char *pUserName) = 0;

    /**
     * @brief 判断用户是否登录
     * @param pUserName 用户名
     * @return 是否登录
     */
    virtual bool IsLogin(const char *pUserName) const = 0;

    /**
     * @brief 获取用户会话ID
     * @param pUserName 用户名
     * @return 会话ID,失败返回0
     */
    virtual uint64_t GetSessionID(const char *pUserName) const = 0;

    /**
     * @brief 修改用户密码
     * @param pUserName 用户名
     * @param pPassword 密码
     * @return 0表示成功,否则失败
     */
    virtual int32_t ModifyPassword(const char *pUserName, const char *pPassword) = 0;

    /**
     * @brief 添加标签
     * @param pTagName 标签名
     * @return 0表示成功,否则失败
     */
    virtual int32_t AddTag(const char *pTagName) = 0;

    /**
     * @brief 删除标签
     * @param pTagName 标签名
     * @return 0表示成功,否则失败
     */
    virtual int32_t RemoveTag(const char *pTagName) = 0;

    /**
     * @brief 获取标签列表
     * @param vTags 标签列表
     * @param uOffset 偏移量
     * @param uLimit 限制数量
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetTags(std::vector<std::string> &vTags, uint32_t uOffset, uint32_t uLimit) const = 0;

    /**
     * @brief 设置用户标签
     * @param pUserName 用户名
     * @param pTagName 标签名
     * @return 0表示成功,否则失败
     */
    virtual int32_t SetTag(const char *pUserName, const char *pTagName) = 0;

    /**
     * @brief 删除用户标签
     * @param pUserName 用户名
     * @param pTagName 标签名
     * @return 0表示成功,否则失败
     */
    virtual int32_t RemoveTag(const char *pUserName, const char *pTagName) = 0;

    /**
     * @brief 获取用户标签列表
     * @param vUserTags 用户标签列表
     * @param pUserName 用户名
     * @param uOffset 偏移量
     * @param uLimit 限制数量
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetUserTags(std::vector<std::string> &vUserTags, const char *pUserName, uint32_t uOffset, uint32_t uLimit) const = 0;

    /**
     * @brief 获取用户管理器统计信息
     * @param strStats 统计信息
     * @return 0表示成功,否则失败
     */
    virtual int32_t GetStats(std::string &strStats) const = 0;
};

namespace config
{

/* ============================== 用户管理器配置 ============================== */
constexpr const char *kSection = "user_manager";       // 配置文件中的节名，类型: string
constexpr const char *kDatabasePath = "database_path"; // 数据库路径，类型: string
constexpr const char *kIsCrypt = "is_crypt";           // 是否加密，类型: bool

}

}
}

#endif // __LITE_DRIVE_USER_MANAGER_H__
