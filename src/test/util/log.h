#pragma once

#include <format>
#include <string>

/*
    Log types represented in enums, strings and their prefix colours
*/
enum LogType
{
    LOG_VERBOSE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CLIENT
};
inline const char* logTypeNames[]{"VERBOSE", "INFO", "WARNING", "ERROR", "CLIENT"};
inline const char* logTypeColors[]{"\x1b[34;40m", "\x1b[32;40m", "\x1b[30;43m", "\x1b[97;41m", "\x1b[97;102"};

#define DECL_LOGGER(NAME, TYPE)                                                                                        \
    template <typename... Args> inline void NAME(const std::format_string<Args...> fmt, Args&&... args)                \
    {                                                                                                                  \
        Write(TYPE, std::vformat(fmt.get(), std::make_format_args(args...)));                                          \
    }

namespace Log
{
#ifdef _WIN32
    /*
        Helper function that enables ANSI escape sequences for colours
    */
    void SetupConsole();
#endif

    /*
        Prints log to console
    */
    void Write(LogType type, std::string message);

    /*
        Logger functions for each type
    */
    DECL_LOGGER(Info, LOG_INFO);
    DECL_LOGGER(Warning, LOG_WARNING);
    DECL_LOGGER(Error, LOG_ERROR);
    DECL_LOGGER(Client, LOG_CLIENT);

#ifdef _DEBUG
    DECL_LOGGER(Verbose, LOG_VERBOSE);
#else
    #define LogVerbose(...)
#endif
}
