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
        auto sources = MakeMake::scan(std::filesystem::current_path(), {".c", ".cpp"});

        MakeMake::Target target;
        target.setName(std::filesystem::current_path().filename());
        target.setSources(sources);
        printf("%s\n", target.makefile().c_str());
        MakeMake::writeFile("Makefile", target.makefile());
        return EXIT_SUCCESS;
    } 
    else 
    {
        std::string install = "install: all\n";
        std::string all = "all: ";
        std::string clean = "clean: \n";
        std::string data;
        MakeMake::Config config;

        auto targets = config.load(path);
        for (auto& target :targets)
        {
            all += target.name();
            clean += "\t" + target.cmdClean();
            install += "\t" + target.cmdInstall();
            data += target.makefile();
        }

        data = MakeMake::strJoin({".PHONY: all install clean", all, install, clean, data}, "\n\n");

        MakeMake::writeFile("Makefile", data);
    }
}