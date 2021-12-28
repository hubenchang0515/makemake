#ifndef MAKEMAKE_UTILS_H
#define MAKEMAKE_UTILS_H

#include <cstdio>
#include <cstdint>
#include <cstddef>

#include <vector>
#include <string>
#include <set>
#include <filesystem>

namespace MakeMake
{

const size_t MAKEMAKE_IOBUFF_SIZE = 256;

/**************************************
 * @brief 清楚字符串两端的空白字符
 * @param[in] str 原始字符串
 * @return 清除空白后的字符串
 * ************************************/
std::string trimSpace(const std::string& str) noexcept;

/**************************************
 * @brief 同步执行一个命令并读取输出
 * @param[in] cmd 要执行的命令
 * @return 命令的输出
 * ************************************/
std::string execute(const std::string& cmd) noexcept;

/**************************************
 * @brief 生成一条规则
 * @param[in] compiler 要使用的编译器
 * @param[in] src 编译的源文件
 * @return 命令的输出
 * ************************************/
std::string rule(const std::string& compiler, const std::string& src) noexcept;

/**************************************
 * @brief 扫描目录,获取指定后缀的文件列表
 * @param[in] dir 要扫描的目录
 * @param[in] ext 文件后缀
 * @return 该目录下符合后缀的文件列表
 * ************************************/
std::vector<std::string> scan(const std::string& dir, const std::set<std::string>& ext) noexcept;

/**************************************
 * @brief 判断一个路径是否存在
 * @param[in] path 要判断的路径
 * @return 是否存在
 * ************************************/
bool exist(const std::string& path) noexcept;

/**************************************
 * @brief 将一组字符串展开成一个字符串
 * @param[in] strs 要展开的字符串vector
 * @return 展开的字符串
 * ************************************/
bool strJoin(const std::vector<std::string>& strs) noexcept;

}; // namespace MakeMake

#endif // MAKEMAKE_UTILS_H