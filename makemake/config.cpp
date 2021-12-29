#include "config.h"
#include "utils.h"

#include <rapidjson/document.h>

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
    else
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