#include "utils.h"
#include "target.h"
#include "config.h"

static const std::string configFile = "./makemake.json";

int main(int argc, char* argv[])
{
    std::filesystem::path path{configFile};

    if (argc == 2 && std::string{argv[1]} == std::string{"init"})
    {
        MakeMake::Config config;
        auto name = std::filesystem::current_path().filename();
        auto sources = MakeMake::scan(std::filesystem::current_path(), {".c", ".cpp"});
        auto data = config.init(name, sources);
        MakeMake::writeFile("makemake.json", data);
        return EXIT_SUCCESS;
    }
    else if (argc == 1 && !std::filesystem::exists(path)) // no makemake.json, default
    {
        auto sources = MakeMake::scan(std::filesystem::current_path(), {".c", ".cpp"});

        MakeMake::Target target;
        target.setName(std::filesystem::current_path().filename());
        target.setSources(sources);
        auto data = target.makefile() + "clean:\n\t" + target.cmdClean();
        MakeMake::writeFile("Makefile", data);
        return EXIT_SUCCESS;
    } 
    else if (argc == 1)
    {
        std::string install = "install: all\n";
        std::string all = "all: ";
        std::string clean = "clean: \n";
        std::string data;
        MakeMake::Config config;

        auto targets = config.load(path);
        for (auto& target :targets)
        {
            all += target.name() + " ";
            if (!target.cmdClean().empty())
                clean += "\t" + target.cmdClean() + "\n";
            if (!target.cmdInstall().empty())
                install += "\t" + target.cmdInstall() + "\n";
            data += target.makefile();
        }

        data = MakeMake::strJoin({".PHONY: all install clean", all, install, clean, data}, "\n\n");

        MakeMake::writeFile("Makefile", data);
    }
    else
    {
        printf("Usage: \n");
        printf("\t%s init : to generate a default 'makemake.json'\n", argv[0]);
        printf("\t%s      : to generate 'Makefile' according to 'makemake.json'\n", argv[0]);
        return EXIT_FAILURE;
    }
}