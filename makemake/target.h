#ifndef MAKEMAKE_TARGET_H
#define MAKEMAKE_TARGET_H

#include "utils.h"

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

    std::string name() const noexcept;
    void setName(const std::string& name) noexcept;

    std::vector<std::string> sources() const noexcept;
    void setSources(const std::vector<std::string>& sources) noexcept;

    std::vector<std::string> depends() const noexcept;
    void setDepends(const std::vector<std::string>& depends) noexcept;

    std::string cmd() const noexcept;
    void setCmd(const std::string& cmd) noexcept;

    std::string cc() const noexcept;
    void setCc(const std::string& cc) noexcept;

    std::string cxx() const noexcept;
    void setCxx(const std::string& cxx) noexcept;

    std::string cflags() const noexcept;
    void setCflags(const std::string& cflags) noexcept;

    std::string cxxflags() const noexcept;
    void setCxxflags(const std::string& libs) noexcept;

    std::string ar() const noexcept;
    void setAr(const std::string& ar) noexcept;

    std::string arflags() const noexcept;
    void setArflags(const std::string& arflags) noexcept;

    std::string libs() const noexcept;
    void setLibs(const std::string& cxxflags) noexcept;

    std::string install() const noexcept;
    void setInstall(const std::string& install) noexcept;

    /***********************************
     * @brief 根据 sources 生成 objects 列表
     * @return objects 列表
     * *********************************/
    std::vector<std::string> objects() const noexcept;

    /***********************************
     * @brief 生成 Makefile 的内容
     * @return Makefile 的内容
     * *********************************/
    std::string makefile() const noexcept;

    /***********************************
     * @brief 生成 install 指令
     * @return install 指令
     * *********************************/
    std::string cmdInstall() const noexcept;

    /***********************************
     * @brief 生成 clean 指令
     * @return clean 指令
     * *********************************/
    std::string cmdClean() const noexcept;

private:
    std::string m_name;
    std::vector<std::string> m_sources;
    std::vector<std::string> m_depends;
    std::string m_cmd;

    std::string m_cc = "gcc";
    std::string m_cxx = "g++";
    std::string m_cflags = "";
    std::string m_cxxflags = "";
    std::string m_ar = "ar";
    std::string m_arflags = "rcs";

    Type        m_type = Type::executable;
    std::string m_libs = "";
    std::string m_install = "";

    std::string m_linker;

}; // class Target

}; // namespace MakeMake

#endif // MAKEMAKE_TARGET_H