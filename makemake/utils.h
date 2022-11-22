#ifndef MAKEMAKE_UTILS_H
#define MAKEMAKE_UTILS_H

#include "platform.h"

#include <cstdio>
#include <cstdint>
#include <cstddef>

#include <vector>
#include <string>
#include <set>
#include <filesystem>

namespace MakeMake
{

constexpr static const size_t MAKEMAKE_IOBUFF_SIZE = 256;
static const std::set<std::string> cExts = {".c"};
static const std::set<std::string> cppExts = {".cpp", ".cxx", ".cc"};
static const std::set<std::string> cudaExts = {".cu"};
static const std::set<std::string> srcExts = {".c", ".cpp", ".cxx", ".cc", ".cu"};

/**************************************
 * @brief 判断文件扩展名是否在范围内
 * @param[in] filename 文件名
 * @param[in] exts 扩展名列表
 * @return 是否在范围内
 * ************************************/
bool isFileExtIn(const std::string& filename, const std::set<std::string>& exts) noexcept;

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
 * @param[in] sp 插入的分割字符串
 * @return 展开的字符串
 * ************************************/
std::string strJoin(const std::vector<std::string>& strs, const std::string& sp) noexcept;

/**************************************
 * @brief 读取整个文件
 * @param[in] file 要读取的文件
 * @return 读取到的内容
 * ************************************/
std::string readFile(const std::string& file) noexcept;

/**************************************
 * @brief 写文件
 * @param[in] file 要读取的文件
 * @param[in] data 要写的内容
 * @return 读取到的内容
 * ************************************/
bool writeFile(const std::string& file, const std::string& data) noexcept;

}; // namespace MakeMake

#endif // MAKEMAKE_UTILS_H