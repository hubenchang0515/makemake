#include "target.h"

namespace MakeMake
{

Target::Target() noexcept 
{
    set("cc", std::string{"gcc"});
    set("cxx", std::string("g++"));
    set("cflags", std::string(""));
    set("cxxflags", std::string(""));
    set("ar", std::string("ar"));
    set("arflags", std::string("rcs"));
    set("type", MakeMake::Target::Type::executable);
    set("libs", std::string(""));
    set("install", std::string(""));
    set("cmd", std::string(""));
}

std::any Target::get(const std::string& key) const noexcept
{
    auto value = m_datas.find(key);
    if (value != m_datas.end())
        return value->second;
    return nullptr;
}

void Target::set(const std::string& key, const std::any& value) noexcept
{
    m_datas[key] = value;
}

std::string Target::getString(const std::string& key) const noexcept
{
    try
    {
        return std::any_cast<std::string>(get(key));
    }
    catch(std::bad_any_cast&)
    {
        return "";
    }
}
std::vector<std::string> Target::getStrVec(const std::string& key) const noexcept
{
    try
    {
        return std::any_cast<std::vector<std::string>>(get(key));
    }
    catch(std::bad_any_cast&)
    {
        return {};
    }
}

/***********************************
 * @brief 解析 sources 中包含的文件夹
 * *********************************/
void Target::parseDir() noexcept
{
    auto sources = getStrVec("sources");
    decltype(sources) newSources;

    for (auto& src : sources)
    {
        std::filesystem::path path{src};
        if (std::filesystem::is_directory(path))
        {
            auto found = scan(path.string(), MakeMake::srcExts);
            for (auto s : found)
            {
                auto dir = path;
                newSources.push_back(dir.append(s).string());
            }
        }
        else
        {
            newSources.push_back(src);
        }
    }

    set("sources", newSources);
}

/**********************************************
 * @brief 根据 sources 生成 objects 列表
 * @return objects 列表
 * ********************************************/
std::vector<std::string> Target::objects() noexcept
{
    std::vector<std::string> objs;
    for (auto& src : getStrVec("sources"))
    {
        std::filesystem::path path{src};
        if (srcExts.find(path.extension().string()) == srcExts.end())
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
std::string Target::makefile() noexcept
{
    std::string str;
    auto objs = strJoin(objects(), " ");
    auto deps = strJoin(getStrVec("depends"), " ");
    str += strJoin({getString("name"), ":", objs, deps, "\n"}, " ");
    auto linker = m_linker();

    switch (std::any_cast<Type>(m_datas["type"]))
    {
    case Type::executable:
        str += "\t" + strJoin({linker, "-o", "$@", "$^", getString("libs"), "\n\n"}, " ");
        break;

    case Type::shared:
        str += "\t" + strJoin({linker, "--shared", "-o", "$@", "$^", getString("libs"), "\n\n"}, " ");
        break;

    case Type::archive:
        str += "\t" + strJoin({getString("ar"), getString("arflags"), "$@", "$^", "\n\n"}, " ");
        break;

    case Type::install:
    case Type::other:
        str += "\t" + strJoin({getString("cmd"), "\n\n"}, " ");
        return str;
    }

    for (const auto& src : getStrVec("sources"))
    {
        std::filesystem::path path{src};
        if (path.extension() == ".c")
        {
            str += rule(getString("cc") + " " + getString("cflags"), src) + "\n";
            str += "\t" + strJoin({getString("cc"), "-c", src, getString("cflags"), "\n\n"}, " ");
        }
        else if(cppExts.find(path.extension().string()) != cppExts.end())
        {
            str += rule(getString("cxx") + " " + getString("cxxflags"), src) + "\n";
            str += "\t" + strJoin({getString("cxx"), "-c ", src, getString("cxxflags"), "\n\n"}, " ");
        }
        else
        {
            fprintf(stderr, "unknow file '%s'\n", src.c_str());
            continue;
        }
    }

    return str;
}

/***********************************
 * @brief 生成 install 指令
 * @return install 指令
 * *********************************/
std::string Target::cmdInstall() noexcept
{
    if (getString("install").empty())
        return "";

    switch (std::any_cast<Type>(m_datas["type"]))
    {
    case Type::executable:
        return strJoin({"install", "-m0755", getString("name"), getString("install")}, " ");

    case Type::shared:
        return strJoin({"install", "-m0644", getString("name"), getString("install")}, " ");

    case Type::archive:
        return strJoin({"install", "-m0644", getString("name"), getString("install")}, " ");
    
    case Type::install:
        return getString("install");

    case Type::other:
        return strJoin({"install", "-m0644", getString("name"), getString("install")}, " ");
    }

    return "";
}

/***********************************
 * @brief 生成 clean 指令
 * @return clean 指令
 * *********************************/
std::string Target::cmdClean() noexcept
{
    auto objs = strJoin(objects(), " ");
    if (objs.empty())
        return "";
#ifndef MAKEMAKE_WINDOWS
    return strJoin({"rm", "-f", objs}, " ");
#else
    return strJoin({"del", "/Q", objs}, " ");
#endif // MAKEMAKE_WINDOWS
}

/***********************************
 * @brief 获取连接器
 * @return 连接器
 * *********************************/
std::string Target::m_linker() const noexcept
{
    for (const auto& s : getStrVec("sources"))
    {
        std::filesystem::path path{s};
        if (cppExts.find(path.extension().string()) != cppExts.end())
        {
            return getString("cxx");
        }
    }

    return getString("cc");
}

}; // namespace MakeMake