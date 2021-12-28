#ifndef MAKEMAKE_GENERATOR_H
#define MAKEMAKE_GENERATOR_H

#include "utils.h"

namespace MakeMake
{

class Generator
{
public:
    Generator() noexcept;

private:
    std::string cCompiler;
    std::string cppCompiler;

}; // class Generator

}; // namespace MakeMake

#endif // MAKEMAKE_GENERATOR_H