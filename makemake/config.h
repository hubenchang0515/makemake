#ifndef MAKEMAKE_CONFIG_H
#define MAKEMAKE_CONFIG_H

#include <map>

#include <rapidjson/document.h>

#include "target.h"

namespace MakeMake
{

class Config
{
public:
    /**************************************
     * @brief 加载配置文件
     * @param[in] file 配置文件
     * @return 构建目标
     * ************************************/
    std::vector<Target> load(const std::string& file) const noexcept;

private:
    /**************************************
     * @brief 加载目标
     * @param[in] target 将配置写入这个目标
     * @param[in] doc JSON子文档
     * @return 构建目标
     * ************************************/
    void loadTarget(Target& target, const rapidjson::Value& doc) const noexcept;
}; // class Config

}; // namespace MakeMake

#endif // MAKEMAKE_CONFIG_H