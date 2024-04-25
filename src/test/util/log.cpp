#include "log.h"

#include <iostream>

#ifdef _WIN32
#include <consoleapi.h>
#include <processenv.h>

// from: WinBase.h
#define STD_OUTPUT_HANDLE ((DWORD)-11)

void Log::SetupConsole()
{
    HANDLE conHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD conMode = 0;
    GetConsoleMode(conHandle, &conMode);
    conMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(conHandle, conMode);
}
#endif

void Log::Write(LogType type, std::string message)
{
    std::cout << logTypeColors[type] << "[" << logTypeNames[type] << "]"
              << "\x1b[0m " << message << std::endl;
}