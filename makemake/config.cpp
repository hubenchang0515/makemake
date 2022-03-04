#include "config.h"
#include "utils.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace MakeMake
{

std::vector<Target> Config::load(const std::string& file) const noexcept
{
    std::vector<Target> targets;

    std::string data = readFile(file);
    rapidjson::Document doc;
    doc.Parse(data.c_str());

    if (!doc.HasMember("targets") || !doc["targets"].IsArray())
    {
        fprintf(stderr, "no targets.\n");
        return targets;
    }

    for (auto& targetDoc : doc["targets"].GetArray())
    {
        auto& target = targets.emplace_back();
        loadTarget(target, targetDoc);
    }

    return targets;
}

/**************************************
 * @brief 生成默认的配置文件
 * @param[in] name 目标名称
 * @param[in] sources 源文件
 * @return 生成的配置数据
 * ************************************/
std::string Config::init(const std::string& name, const std::vector<std::string>& sources) const noexcept
{
    rapidjson::StringBuffer strBuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuf);

    writer.StartObject(); // root
    writer.Key("targets");
    writer.StartArray(); // targets

    writer.StartObject(); // target

    writer.Key("name");
    writer.String(name.c_str());

    writer.Key("type");
    writer.String("executable");

    writer.Key("cc");
    writer.String("gcc");

    writer.Key("cxx");
    writer.String("g++");

    writer.Key("cflags");
    writer.String("-O2 -W -Wall");

    writer.Key("cxxflags");
    writer.String("-O2 -W -Wall");

    writer.Key("ar");
    writer.String("ar");

    writer.Key("arflags");
    writer.String("rcs");

    writer.Key("libs");
    writer.String("");

    writer.Key("install");
    writer.String("");

    writer.Key("cmd");
    writer.String("");

    writer.Key("sources");
    writer.StartArray(); // sources

    for (auto& src : sources)
    {
        writer.String(src.c_str());
    }

    writer.EndArray(); // sources

    writer.Key("depends");
    writer.StartArray(); // depends
    writer.EndArray(); // depends

    writer.EndObject(); // target
    writer.EndArray(); // targets
    writer.EndObject(); // root

    return strBuf.GetString();
}

/**************************************
 * @brief 加载目标
 * @param[in] target 将配置写入这个目标
 * @param[in] doc JSON子文档
 * @return 构建目标
 * ************************************/
void Config::loadTarget(Target& target, const rapidjson::Value& json) const noexcept
{
    auto getString = [&json](const std::string& key) -> std::string{
        auto keyOs = key + "." + OS_NAME;
        if (json.HasMember(keyOs.c_str()) && json[keyOs.c_str()].IsString())
            return json[keyOs.c_str()].GetString();
        
        if (json.HasMember(key.c_str()) && json[key.c_str()].IsString())
            return json[key.c_str()].GetString();
        
        return "";
    };

    auto configString = [&target, &getString](const std::string& key) {
        std::string value = getString(key);
        if (value == "")
            return;
        target.set(key, value);
    };

    static std::vector<std::string> configStringList = {
        "name", "cc", "cxx", "cflags", "cxxflags",
        "ar", "arflags", "libs", "install", "cmd", 
        "type"
    };

    for (auto& key : configStringList)
    {
        configString(key);
    }

    if (getString("type") != "")
    {
        static std::map<std::string, Target::Type> types{
            {"executable", Target::Type::executable},
            {"shared", Target::Type::shared},
            {"archive", Target::Type::archive},
            {"other", Target::Type::other},
        };
        target.set("type", types[getString("type")]);
    }

    auto getStrVec = [&json](const std::string& key) -> std::vector<std::string>{
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

    target.set("sources", getStrVec("sources"));
    target.set("depends", getStrVec("depends"));
}


}; // namespace MakeMake