#include "target.h"

namespace MakeMake
{

// string 类型的字段
static std::vector<std::string> configStringList = {
    "name", "cc", "cxx", "cflags", "cxxflags",
    "ar", "arflags", "libs", "install", "uninstall",
    "cmd"
};

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
    set("uninstall", std::string(""));
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

void Target::load(const rapidjson::Value& json) noexcept
{
    auto jsonGetString = [&json](const std::string& key) -> std::string{
        auto keyOs = key + "." + OS_NAME;
        if (json.HasMember(keyOs.c_str()) && json[keyOs.c_str()].IsString())
            return json[keyOs.c_str()].GetString();
        
        if (json.HasMember(key.c_str()) && json[key.c_str()].IsString())
            return json[key.c_str()].GetString();
        
        return "";
    };

    auto configString = [this, &jsonGetString](const std::string& key) {
        std::string value = jsonGetString(key);
        if (value == "")
            return;
        this->set(key, value);
    };

    // 加载 type 转换为枚举
    if (jsonGetString("type") != "")
    {
        static std::map<std::string, Target::Type> types{
            {"executable", Target::Type::executable},
            {"shared", Target::Type::shared},
            {"archive", Target::Type::archive},
            {"other", Target::Type::other},
        };
        this->set("type", types[jsonGetString("type")]);
    }

    // 加载 string 类型的字段
    for (auto& key : configStringList)
    {
        configString(key);
    }

    // 配置 string array 类型的字段
    auto jsonGetStringArray = [&json](const std::string& key) -> std::vector<std::string>{
        auto keyOs = key + "." + OS_NAME;
        std::string keySelect;
        if (json.HasMember(keyOs.c_str()) && json[keyOs.c_str()].IsArray())
            keySelect = keyOs;
        else if (json.HasMember(key.c_str()) && json[key.c_str()].IsArray())
            keySelect = key;
        else
            return {};

        auto jsonArray = json[keySelect.c_str()].GetArray();
        std::vector<std::string> strVec;
        for (auto& s : jsonArray)
        {
            strVec.emplace_back(s.GetString());
        }

        return strVec;
    };

    this->set("sources", jsonGetStringArray("sources"));
    this->set("depends", jsonGetStringArray("depends"));
}

void Target::dump(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const noexcept
{
    writer.StartObject(); // target

    // 写 type 字段
    writer.Key("type");
    switch (std::any_cast<Type>(m_datas.at("type")))
    {
    case Type::executable:
        writer.String("executable");
        break;

    case Type::shared:
        writer.String("shared");
        break;

    case Type::archive:
        writer.String("archive");
        break;

    default:
        writer.String("other");
        break;
    }

    // 写 string 类型的字段
    for (auto& key : configStringList)
    {
        writer.Key(key.c_str());
        writer.String(this->getString(key).c_str());
    }

    // 写 sources
    writer.Key("sources");
    writer.StartArray(); // sources
    auto sources = getStrVec("sources");
    for (auto& src : sources)
    {
        writer.String(src.c_str());
    }
    writer.EndArray(); // sources

    // 写 depends
    writer.Key("depends");
    writer.StartArray(); // depends
    auto depends = getStrVec("depends");
    for (auto& src : depends)
    {
        writer.String(src.c_str());
    }
    writer.EndArray(); // depends

    writer.EndObject(); // target

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

    return strJoin({"\t", getString("install")}, " ");
}

/***********************************
 * @brief 生成 uninstall 指令
 * @return uninstall 指令
 * *********************************/
std::string Target::cmdUninstall() noexcept
{
    if (getString("uninstall").empty())
        return "";

    return strJoin({"\t", getString("uninstall")}, " ");
}

/***********************************
 * @brief 生成 clean 指令
 * @return clean 指令
 * *********************************/
std::string Target::cmdClean() noexcept
{
    std::string junk;
    if (std::any_cast<Type>(m_datas["type"]) == Type::other)
        junk = getString("name");
    else
        junk = strJoin(objects(), " ");

    if (junk.empty())
        return "";
#ifndef MAKEMAKE_WINDOWS
    return strJoin({"\t", "rm", "-f", junk}, " ");
#else
    return strJoin({"\t", "del", "/Q", junk}, " ");
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