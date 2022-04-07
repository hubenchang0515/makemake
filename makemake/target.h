#ifndef MAKEMAKE_TARGET_H
#define MAKEMAKE_TARGET_H

#include "platform.h"
#include "utils.h"

#include <map>
#include <any>

namespace MakeMake
{


class Target
{
public:

    enum class Type{
        executable = 0,
        shared,
        archive,
        other,
    };

    Target() noexcept;
    ~Target() noexcept = default;

    std::any get(const std::string& key) const noexcept;
    void set(const std::string& key, const std::any& value) noexcept;

    std::string getString(const std::string& key) const noexcept;
    std::vector<std::string> getStrVec(const std::string& key) const noexcept;

    /***********************************
     * @brief 解析 sources 中包含的文件夹
     * *********************************/
    void parseDir() noexcept;

    /***********************************
     * @brief 根据 sources 生成 objects 列表
     * @return objects 列表
     * *********************************/
    std::vector<std::string> objects() noexcept;

    /***********************************
     * @brief 生成 Makefile 的内容
     * @return Makefile 的内容
     * *********************************/
    std::string makefile() noexcept;

    /***********************************
     * @brief 生成 install 指令
     * @return install 指令
     * *********************************/
    std::string cmdInstall() noexcept;

    /***********************************
     * @brief 生成 uninstall 指令
     * @return uninstall 指令
     * *********************************/
    std::string cmdUninstall() noexcept;

    /***********************************
     * @brief 生成 clean 指令
     * @return clean 指令
     * *********************************/
    std::string cmdClean() noexcept;

private:
    std::map<std::string, std::any> m_datas;

    /***********************************
     * @brief 获取连接器
     * @return 连接器
     * *********************************/
    std::string m_linker() const noexcept;

}; // class Target

}; // namespace MakeMake

#endif // MAKEMAKE_TARGET_H