#ifndef MAKEMAKE_CONFIG_H
#define MAKEMAKE_CONFIG_H

#include <map>

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
    std::vector<Target> load(const std::string& file);

private:
    std::string m_data;
}; // class Config

}; // namespace MakeMake

#endif // MAKEMAKE_CONFIG_H