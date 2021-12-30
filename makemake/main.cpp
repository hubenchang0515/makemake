#include "utils.h"
#include "target.h"
#include "config.h"

static const std::string configFile = "./makemake.json";

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string{argv[1]} == std::string{"init"})
    {
        MakeMake::Config config;
        auto name = std::filesystem::current_path().filename();
        auto sources = MakeMake::scan(std::filesystem::current_path(), MakeMake::srcExts);
        auto data = config.init(name, sources);
        MakeMake::writeFile("makemake.json", data);
        return EXIT_SUCCESS;
    }
    else if (argc == 1)
    {
        std::filesystem::path path{configFile};
        std::vector<MakeMake::Target> targets;
        if (!std::filesystem::exists(path)) // no makemake.json
        {
            auto sources = MakeMake::scan(std::filesystem::current_path(), MakeMake::srcExts);
            if (sources.empty())
            {
                fprintf(stderr, "cannot find makemake.json or source files.\n");
                return EXIT_FAILURE;
            }
            auto& target = targets.emplace_back();
            target.setName(std::filesystem::current_path().filename());
            target.setSources(sources);
        }
        else
        {
            MakeMake::Config config;
            targets = config.load(path);
        }

        std::string install = "install: all";
        std::string all = "all:";
        std::string clean = "clean:";
        std::string data;
        for (auto& target :targets)
        {
            all += " " + target.name();
            if (!target.cmdClean().empty())
                clean += "\n\t" + target.cmdClean();
            if (!target.cmdInstall().empty())
                install += "\n\t" + target.cmdInstall();
            data += target.makefile();
        }
        data = MakeMake::strJoin({".PHONY: all install clean", all, install, clean, data}, "\n\n");

        MakeMake::writeFile("Makefile", data);
    }
    else
    {
        printf("Usage: \n");
        printf("\t%s init : to generate a 'makemake.json' template\n", argv[0]);
        printf("\t%s      : to generate a 'Makefile' based on 'makemake.json'\n", argv[0]);
        printf("\n");
        printf("Makemake is used to make makefile of tiny project.\n");
        printf("Get more infomation from 'https://github.com/hubenchang0515/makemake'.\n");
        return EXIT_FAILURE;
    }
}