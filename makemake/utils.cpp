#include "utils.h"

namespace MakeMake
{

/**************************************
 * @brief 清楚字符串两端的空白字符
 * @param[in] str 原始字符串
 * @return 清除空白后的字符串
 * ************************************/
std::string trimSpace(const std::string& str) noexcept
{
    auto start = str.find_first_not_of(" \t\r\n");
    if (start == str.npos)
        return "";
    auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

/**************************************
 * @brief 同步执行一个命令并读取输出
 * @param[in] cmd 要执行的命令
 * @return 命令的输出
 * ************************************/
std::string execute(const std::string& cmd) noexcept
{
    FILE* fp = popen(cmd.c_str(), "r");
    if (fp == nullptr)
    {
        return "";
    }

    std::string ret;
    char buff[MAKEMAKE_IOBUFF_SIZE];
    do
    {
        size_t len = fread(buff, 1, MAKEMAKE_IOBUFF_SIZE, fp);
        ret.append(buff, len);
    }while(!feof(fp));

    fclose(fp);

    return ret;
}

/**************************************
 * @brief 生成一条编译规则
 * @param[in] compiler 要使用的编译器
 * @param[in] src 编译的源文件
 * @return 命令的输出
 * ************************************/
std::string rule(const std::string& compiler, const std::string& src) noexcept
{
    return trimSpace(execute(compiler + " -MM " + src));
}

/**************************************
 * @brief 扫描目录,获取指定后缀的文件列表
 * @param[in] dir 要扫描的目录
 * @param[in] exts 文件后缀列表
 * @return 该目录下符合后缀的文件列表
 * ************************************/
std::vector<std::string> scan(const std::string& dir, const std::set<std::string>& exts) noexcept
{
    std::vector<std::string> files;

    std::filesystem::path path{dir};
    if (!std::filesystem::exists(path))
    {
        fprintf(stderr, "%s is not exist.\n", path.c_str());
        return files;
    }

    std::filesystem::directory_entry entry{path};
    if (entry.status().type() != std::filesystem::file_type::directory)
    {
        fprintf(stderr, "%s is not a directory.\n", path.c_str());
        return files;
    }

    std::filesystem::directory_iterator iter{path};
    for (auto& file : iter)
    {
        if (exts.find(file.path().extension()) == exts.end())
            continue;

        files.emplace_back(file.path().filename());
    }

    return files;
}

/**************************************
 * @brief 判断一个路径是否存在
 * @param[in] path 要判断的路径
 * @return 是否存在
 * ************************************/
bool exist(const std::string& path) noexcept
{
    std::filesystem::path fspath{path};
    return std::filesystem::exists(fspath);
}

/**************************************
 * @brief 将一组字符串展开成一个字符串
 * @param[in] strs 要展开的字符串vector
 * @param[in] sp 插入的分割字符串
 * @return 展开的字符串
 * ************************************/
std::string strJoin(const std::vector<std::string>& strs, const std::string& sp) noexcept
{
    if (strs.size() == 0)
        return "";

    std::string ret;
    size_t i = 0;
    for (; i + 1 < strs.size(); i++)
    {
        ret += strs[i] + sp;
    }

    ret += strs[i];

    return ret;
}

/**************************************
 * @brief 读取整个文件
 * @param[in] file 要读取的文件
 * @return 读取到的内容
 * ************************************/
std::string readFile(const std::string& file) noexcept
{
    FILE* fp = fopen(file.c_str(), "rb");
    if (fp == nullptr)
    {
        return "";
    }

    std::string ret;
    char buff[MAKEMAKE_IOBUFF_SIZE];
    do
    {
        size_t len = fread(buff, 1, MAKEMAKE_IOBUFF_SIZE, fp);
        ret.append(buff, len);
    }while(!feof(fp));

    fclose(fp);
    return ret;
}

/**************************************
 * @brief 写文件
 * @param[in] file 要读取的文件
 * @param[in] data 要写的内容
 * @return 读取到的内容
 * ************************************/
bool writeFile(const std::string& file, const std::string& data) noexcept
{
    FILE* fp = fopen(file.c_str(), "w");
    if (fp == nullptr)
    {
        return false;
    }

    fwrite(data.c_str(), data.size(), 1, fp);
    fclose(fp);
    return true;
}

}; // namespace MakeMake