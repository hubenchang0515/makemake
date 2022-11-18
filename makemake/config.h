#ifndef MAKEMAKE_CONFIG_H
#define MAKEMAKE_CONFIG_H

#include "platform.h"

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
     * ************************************/
    void load(const std::string& file) noexcept;

    /**************************************
     * @brief 生成默认的配置文件
     * @param[in] name 目标名称
     * @param[in] sources 源文件列表
     * ************************************/
    void init(const std::string& name, const std::vector<std::string>& sources) noexcept;

    /**************************************
     * @brief 刷新第一个 target 中的源文件列表
     *        保持其它配置不变
     * @param[in] sources 源文件
     * ************************************/
    void refresh(const std::vector<std::string>& sources) noexcept;

    /**************************************
     * @brief 导出 JSON 格式的配置数据
     * @return 生成的配置数据
     * ************************************/
    std::string dumps() const noexcept;

    /**************************************
     * @brief 读取 targets
     * @return targets 的只读引用
     * ************************************/
    const std::vector<Target>& targets() const noexcept;

private:
    std::vector<Target> m_targets;
}; // class Config

}; // namespace MakeMake

#endif // MAKEMAKE_CONFIG_H