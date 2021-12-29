#include "utils.h"
#include "target.h"
#include "config.h"

static const std::string configFile = "./makemake.json";

int main(int argc, char* argv[])
{
    std::filesystem::path path{configFile};

    if (argc > 2)
    {
        printf("Usage: %s [init]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2 && std::string{argv[1]} == std::string{"init"})
    {
        // TODO: 生成默认的 makemake.json
        return EXIT_SUCCESS;
    }

    if (!std::filesystem::exists(path)) // no makemake.json, default
    {
        auto sources = MakeMake::scan("./", {".c", ".cpp"});

        MakeMake::Target target;
        target.setName(std::filesystem::current_path().filename());
        target.setSources(sources);
        target.setCxxflags("-Iinclude -std=c++17 -g");
        target.setLibs("-lstdc++fs");
        printf("%s\n", target.makefile().c_str());
        return EXIT_SUCCESS;
    } 
    else 
    {
        MakeMake::Config config;
        auto targets = config.load(path);
        for (auto& target :targets)
        {
            printf("%s\n", target.makefile().c_str());
        }
    }
}