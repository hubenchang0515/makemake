#include "config.h"
#include "utils.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace MakeMake
{

std::vector<Target> Config::load(const std::string& file)
{
    std::vector<Target> targets;
    std::map<std::string, int> pos;

    m_data = readFile(file);
    rapidjson::Document doc;
    doc.Parse(m_data.c_str());

    auto get = [](const rapidjson::Value& json, const char* key) -> std::string{
        if (json.HasMember(key) && json[key].IsString())
            return json[key].GetString();
        else
            return "";
    };

    for (auto& targetDoc : doc["targets"].GetArray())
    {
        auto& target = targets.emplace_back();

        if (get(targetDoc, "name") != "")
        {
            target.setName(get(targetDoc, "name"));
        }

        if (get(targetDoc, "cc") != "")
        {
            target.setCc(get(targetDoc, "cc"));
        }

        if (get(targetDoc, "cxx") != "")
        {
            target.setCxx(get(targetDoc, "cxx"));
        }

        if (get(targetDoc, "cflags") != "")
        {
            target.setCflags(get(targetDoc, "cflags"));
        }

        if (get(targetDoc, "cxxflags") != "")
        {
            target.setCxxflags(get(targetDoc, "cxxflags"));
        }

        if (get(targetDoc, "ar") != "")
        {
            target.setAr(get(targetDoc, "ar"));
        }

        if (get(targetDoc, "arflags") != "")
        {
            target.setArflags(get(targetDoc, "arflags"));
        }

        if (get(targetDoc, "libs") != "")
        {
            target.setLibs(get(targetDoc, "libs"));
        }

        if (get(targetDoc, "install") != "")
        {
            target.setInstall(get(targetDoc, "install"));
        }

        auto sourcesDoc = targetDoc["sources"].GetArray();
        std::vector<std::string> sources;
        for (auto& s : sourcesDoc)
        {
            sources.emplace_back(s.GetString());
        }
        target.setSources(sources);
    }

    return targets;
}


}; // namespace MakeMake