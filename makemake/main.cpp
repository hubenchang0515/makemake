#include "utils.h"
#include "target.h"
#include "config.h"

static const std::string configFile = "./makemake.json";

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string{argv[1]} == std::string{"init"})
    {
        MakeMake::Config config;
        auto name = std::filesystem::current_path().filename().string();
        auto sources = MakeMake::scan(std::filesystem::current_path().string(), MakeMake::srcExts);
        config.init(name, sources);
        MakeMake::writeFile("makemake.json", config.dumps());
        return EXIT_SUCCESS;
    }
    if (argc == 2 && std::string{argv[1]} == std::string{"refresh"})
    {
        std::filesystem::path path{configFile};
        MakeMake::Config config;
        auto sources = MakeMake::scan(std::filesystem::current_path().string(), MakeMake::srcExts);
        if (!std::filesystem::exists(path)) // no makemake.json
        {
            auto name = std::filesystem::current_path().filename().string();
            config.init(name, sources);
        }
        else
        {
            config.load(path.string());
            config.refresh(sources);
        }
        MakeMake::writeFile("makemake.json", config.dumps());
        return EXIT_SUCCESS;
    }
    else if (argc == 1)
    {
        std::filesystem::path path{configFile};
        std::vector<MakeMake::Target> targets;
        if (!std::filesystem::exists(path)) // no makemake.json
        {
            auto sources = MakeMake::scan(std::filesystem::current_path().string(), MakeMake::srcExts);
            if (sources.empty())
            {
                fprintf(stderr, "cannot find makemake.json or source files.\n");
                return EXIT_FAILURE;
            }
            auto& target = targets.emplace_back();
            target.set("name", std::filesystem::current_path().filename().string());
            target.set("sources", sources);
        }
        else
        {
            MakeMake::Config config;
            config.load(path.string());
            targets = config.targets();
        }


        std::string data;
        std::string install = "install: all";
        std::string uninstall = "uninstall:";
        std::string all = "all:";
        std::string clean = "clean:";
        for (auto& target :targets)
        {
            target.parseDir();
            all += " " + std::any_cast<std::string>(target.get("name"));
            if (!target.cmdClean().empty())
                clean += "\n" + target.cmdClean();
            if (!target.cmdInstall().empty())
                install += "\n" + target.cmdInstall();
            if (!target.cmdUninstall().empty())
                uninstall += "\n" + target.cmdUninstall();
            data += target.makefile();
        }
        data = MakeMake::strJoin({
            "# Generated by [MakeMake](https://github.com/hubenchang0515/makemake)",
            ".PHONY: all install uninstall clean", all, install, uninstall, clean, data}, "\n\n");

        MakeMake::writeFile("Makefile", data);
    }
    else
    {
        printf("Usage: \n");
        printf("\t%s init    : to generate a 'makemake.json' template\n", argv[0]);
        printf("\t%s refresh : to refresh sources in the 'makemake.json'\n", argv[0]);
        printf("\t%s         : to generate a 'Makefile' based on 'makemake.json'\n", argv[0]);
        printf("\n");
        printf("Makemake is used to make makefile of tiny project.\n");
        printf("Get more infomation from 'https://github.com/hubenchang0515/makemake'.\n");
        return EXIT_FAILURE;
    }
}