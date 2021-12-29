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
void Config::loadTarget(Target& target, const rapidjson::Value& doc) const noexcept
{
    auto get = [](const rapidjson::Value& json, const char* key) -> std::string{
        if (json.HasMember(key) && json[key].IsString())
            return json[key].GetString();
        else
            return "";
    };

    if (get(doc, "name") != "")
    {
        target.setName(get(doc, "name"));
    }
    else
    {
        fprintf(stderr, "target has no name.\n");
    }

    if (get(doc, "cc") != "")
    {
        target.setCc(get(doc, "cc"));
    }

    if (get(doc, "cxx") != "")
    {
        target.setCxx(get(doc, "cxx"));
    }

    if (get(doc, "cflags") != "")
    {
        target.setCflags(get(doc, "cflags"));
    }

    if (get(doc, "cxxflags") != "")
    {
        target.setCxxflags(get(doc, "cxxflags"));
    }

    if (get(doc, "ar") != "")
    {
        target.setAr(get(doc, "ar"));
    }

    if (get(doc, "arflags") != "")
    {
        target.setArflags(get(doc, "arflags"));
    }

    if (get(doc, "libs") != "")
    {
        target.setLibs(get(doc, "libs"));
    }

    if (get(doc, "install") != "")
    {
        target.setInstall(get(doc, "install"));
    }

    if (get(doc, "cmd") != "")
    {
        target.setCmd(get(doc, "cmd"));
    }

    if (get(doc, "type") != "")
    {
        static std::map<std::string, Target::Type> types{
            {"executable", Target::Type::executable},
            {"shared", Target::Type::shared},
            {"archive", Target::Type::archive},
            {"other", Target::Type::other},
        };

        target.setType(types[get(doc, "type")]);
    }
    

    if (doc.HasMember("sources") && doc["sources"].IsArray())
    {
        auto sourcesDoc = doc["sources"].GetArray();
        std::vector<std::string> sources;
        for (auto& s : sourcesDoc)
        {
            sources.emplace_back(s.GetString());
        }
        target.setSources(sources);
    }
    else if (target.type() != Target::Type::other)
    {
        fprintf(stderr, "%s has no sources.\n", target.name().c_str());
    }

    if (doc.HasMember("depends") && doc["depends"].IsArray())
    {
        auto dependsDoc = doc["depends"].GetArray();
        std::vector<std::string> depends;
        for (auto& s : dependsDoc)
        {
            depends.emplace_back(s.GetString());
        }
        target.setDepends(depends);
    }
}


}; // namespace MakeMake