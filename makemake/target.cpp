#include "target.h"

namespace MakeMake
{

static const std::set<std::string> cppExts = {".cpp"};
static const std::set<std::string> srcExts = {".c", ".cpp"};

std::string Target::name() const noexcept
{
    return m_name;
}

void Target::setName(const std::string& name) noexcept
{
    m_name = name;
}

std::vector<std::string> Target::sources() const noexcept
{
    return m_sources;
}

void Target::setSources(const std::vector<std::string>& sources) noexcept
{
    
    m_sources = sources;
    for (auto& s :m_sources)
    {
        std::filesystem::path path{s};
        if (cppExts.find(path.extension()) != cppExts.end())
        {
            m_link = m_cxx;
            return;
        }
    }

    m_link = m_cc;
}

std::vector<const Target*> Target::depends() const noexcept
{
    return m_depends;
}

void Target::setDepends(const std::vector<const Target*>& depends) noexcept
{
    m_depends = depends;
}

std::string Target::cmd() const noexcept
{
    return m_cmd;
}

void Target::setCmd(const std::string& cmd) noexcept
{
    m_cmd = cmd;
}

std::string Target::cc() const noexcept
{
    return m_cc;
}

void Target::setCc(const std::string& cc) noexcept
{
    m_cc = cc;
}

std::string Target::cxx() const noexcept
{
    return m_cxx;
}

void Target::setCxx(const std::string& cxx) noexcept
{
    m_cxx = cxx;
}

std::string Target::cflags() const noexcept
{
    return m_cflags;
}

void Target::setCflags(const std::string& cflags) noexcept
{
    m_cflags = cflags;
}

std::string Target::cxxflags() const noexcept
{
    return m_cxxflags;
}

void Target::setCxxflags(const std::string& cxxflags) noexcept
{
    m_cxxflags = cxxflags;
}

std::string Target::libs() const noexcept
{
    return m_libs;
}

void Target::setLibs(const std::string& libs) noexcept
{
    m_libs = libs;
}

std::string Target::ar() const noexcept
{
    return m_ar;
}

void Target::setAr(const std::string& ar) noexcept
{
    m_ar = ar;
}

std::string Target::arflags() const noexcept
{
    return m_arflags;
}

void Target::setArflags(const std::string& arflags) noexcept
{
    m_arflags = arflags;
}

/**********************************************
 * @brief 根据 sources 生成 objects 列表
 * @return objects 列表
 * ********************************************/
std::vector<std::string> Target::objects() const noexcept
{
    std::vector<std::string> objs;
    for (auto& src : m_sources)
    {
        std::filesystem::path path{src};
        if (srcExts.find(path.extension()) == srcExts.end())
        {
            fprintf(stderr, "unknow file '%s'\n", src.c_str());
            continue;
        }
        objs.emplace_back(path.stem().string() + ".o");
    }

    return objs;
}

/***********************************
 * @brief 生成 Makefile 的内容
 * @return Makefile 的内容
 * *********************************/
std::string Target::makefile() const noexcept
{
    std::string str;
    str += m_name + ": ";
    
    auto objs = objects();
    for (const auto& obj : objs)
    {
        str += obj + " ";
    }

    str += "\n\t";
    str += m_link + " -o $@ $^ " + m_libs + "\n";

    for (const auto& src : m_sources)
    {
        std::filesystem::path path{src};
        if (path.extension() == ".c")
        {
            str += rule(m_cc, src);
            str += "\n\t";
            str += m_cc + " -c " + src + " " + m_cflags +"\n";
        }
        else if(cppExts.find(path.extension()) != cppExts.end())
        {
            str += rule(m_cxx, src);
            str += "\n\t";
            str += m_cxx + " -c " + src + " " + m_cxxflags +"\n";
        }
        else
        {
            fprintf(stderr, "unknow file '%s'\n", src.c_str());
            continue;
        }
    }

    return str;
}

}; // namespace MakeMake