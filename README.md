# makemake
Makemake is used to make makefile of tiny project - 用于小型项目的 Makefile 生成器

## Usage / 使用说明

### Generate Configure File Template / 生成配置文件模板

```
$ makemake init
```

It will generate an `makemake.json` template. It will name target same as the directory name, set default compiler and flags, and include all source file (`.c`, `.cpp`) of the directory.  

它将生成 `makemake.json` 的模板，它将目标命名为目录名，设置默认的编译器和编译选项，并包含当前目录下的所有源文件(`.c`，`.cpp`)。  

### Generate Makefile / 生成 Makefile

```
$ makemake
```

If there is a `makemake.json` in the working directory, it will generate a `Makefile` according to the `makemake.json`. Otherwise,  it will generate a default `Makefile` ， that will compile all source file (`.c`, `.cpp`) of the directory and target is an executable file with same name of the directory.

如果工作目录下存在 `makemake.json`，将根据该文件生成一个 `Makefile`。否则将生成一个默认的 `Makefile`，这个默认的 `Makefile` 会使用 GCC 编译目录下的所有源文件(`.c`，`.cpp`)，最终生成一个与目录同名的可执行文件。

### Configure / 配置

```json
{
    "targets" : [
        {
            "name" : "<target name>",
            "type" : "[executable | shared | archive | install | other]",
            "cc" : "<C compiler>",
            "cxx" : "<C++ compiler>",
            "cflags" : "<C compiling flags>",
            "cxxflags" : "<C++ compiling flags>",
            "ar" : "<archive tool>",
            "arflags" : "<archive flags>",
            "libs" : "<link libs>",
            "install" : "<install path>",
            "cmd" : "<command, only for type other>",
            "sources" : [
                // source files
            ],
            "depends" : [
                // other targets
            ]
        }
    ]
}
```