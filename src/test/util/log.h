#pragma once

#include <string>
#include <format>

/*
    TO-DO:
        add subscriptions, log levels, dynamic types
*/

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
        write(TYPE, std::vformat(fmt.get(), std::make_format_args(args...)));                                          \
    }

namespace Log
{
#ifdef _WIN32
    /*
        Helper function that enables ANSI escape sequences for colours
    */
    void setupConsole();
#endif

    /*
        Prints log to console
    */
    void write(LogType type, std::string message);

    /*
        Logger functions for each type
    */
    DECL_LOGGER(info, LOG_INFO);
    DECL_LOGGER(warning, LOG_WARNING);
    DECL_LOGGER(error, LOG_ERROR);
    DECL_LOGGER(client, LOG_CLIENT);
    DECL_LOGGER(verbose, LOG_VERBOSE);

}