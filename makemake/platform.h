#ifndef MAKEMAKE_PLATFORM_H
#define MAKEMAKE_PLATFORM_H

namespace MakeMake
{

// GCC: https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined(__ANDROID__) || defined(__ANDROID_API__)
    static const char* OS_NAME = "android";
#elif defined(__linux__) || defined(linux) || defined(__linux)
    static const char* OS_NAME = "linux";
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
    static const char* OS_NAME = "mac";
#elif defined(BSD) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    static const char* OS_NAME = "bsd";
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
    static const char* OS_NAME = "windows";
#endif

}; // namespace MakeMake

#endif // MAKEMAKE_PLATFORM_H