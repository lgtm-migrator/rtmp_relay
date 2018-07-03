//
//  rtmp_relay
//

#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#else
    #if defined(LOG_SYSLOG)
    #include <sys/syslog.h>
    #endif
#endif
#include "Log.hpp"

namespace relay
{
#ifndef DEBUG
    Log::Level Log::threshold = Log::Level::INFO;
#else
    Log::Level Log::threshold = Log::Level::ALL;
#endif

#if defined(LOG_SYSLOG)
    bool Log::syslogEnabled = true;
#else
    bool Log::syslogEnabled = false;
#endif

    void Log::flush()
    {
        if (!s.empty())
        {
            auto n = std::chrono::system_clock::now();
            auto t = std::chrono::system_clock::to_time_t(n);
            std::tm buffer;
            localtime_s(&buffer, &t);

            if (level == Level::ERR ||
                level == Level::WARN)
                std::cerr << std::put_time(&buffer, "%Y.%m.%d %H:%M:%S") << ": " << s << std::endl;
            else
                std::cout << std::put_time(&buffer, "%Y.%m.%d %H:%M:%S") << ": " << s << std::endl;

#ifdef _WIN32
            wchar_t szBuffer[MAX_PATH];
            MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, szBuffer, MAX_PATH);
            StringCchCatW(szBuffer, sizeof(szBuffer), L"\n");
            OutputDebugStringW(szBuffer);
#elif defined(LOG_SYSLOG)
            if (syslogEnabled)
            {
                int priority = 0;
                switch (level)
                {
                    case Level::ERR: priority = LOG_ERR; break;
                    case Level::WARN: priority = LOG_WARNING; break;
                    case Level::INFO: priority = LOG_INFO; break;
                    case Level::ALL: priority = LOG_DEBUG; break;
                    default: break;
                }
                syslog(priority, "%s", s.c_str());
            }
#endif
            s.clear();
        }
    }
}
