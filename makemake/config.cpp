#include "config.h"
#include "utils.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace MakeMake
{

/**************************************
 * @brief 加载配置文件
 * @param[in] file 配置文件
 * ************************************/
void Config::load(const std::string& file) noexcept
{
    std::string data = readFile(file);
    rapidjson::Document doc;
    doc.Parse(data.c_str());

    if (!doc.HasMember("targets") || !doc["targets"].IsArray())
    {
        fprintf(stderr, "no targets.\n");
    }

    for (auto& targetDoc : doc["targets"].GetArray())
    {
        auto& target = m_targets.emplace_back();
        target.load(targetDoc);
    }
}

/**************************************
 * @brief 生成默认的配置
 * @param[in] name 目标名称
 * @param[in] sources 源文件列表
 * ************************************/
void Config::init(const std::string& name, const std::vector<std::string>& sources) noexcept
{
    auto& target = m_targets.emplace_back();
    target.set("name", name);
    target.set("sources", sources);
}

/**************************************
 * @brief 刷新第一个 target 中的源文件列表
 *        保持其它配置不变
 * @param[in] sources 源文件
 * ************************************/
void Config::refresh(const std::vector<std::string>& sources) noexcept
{
    auto& target = m_targets[0];
    target.set("sources", sources);
}

/**************************************
 * @brief 导出 JSON 格式的配置数据
 * @return 生成的配置数据
 * ************************************/
std::string Config::dumps() const noexcept
{
    rapidjson::StringBuffer strBuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuf);

    writer.StartObject(); // root
    writer.Key("targets");
    writer.StartArray(); // targets

    for (auto& target: m_targets)
    {
        target.dump(writer);
    }

    writer.EndArray(); // targets
    writer.EndObject(); // root

    return strBuf.GetString();
}

/**************************************
 * @brief 读取 targets
 * @return targets 的只读引用
 * ************************************/
const std::vector<Target>& Config::targets() const noexcept
{
    return m_targets;
}

}; // namespace MakeMake