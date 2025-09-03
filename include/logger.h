#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <mutex>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

inline const char *toString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    }
    return "";
}

inline std::mutex &log_mutex()
{
    static std::mutex m;
    return m;
}

template <typename... Args>
inline void logMessage(LogLevel level, const char *func, const char *file, int line, Args &&...args)
{
    std::ostringstream oss;
    if constexpr (sizeof...(args) > 0)
        (oss << ... << std::forward<Args>(args)); // fold + perfect forwarding
    std::lock_guard<std::mutex> lock(log_mutex());
    std::cerr << "[" << toString(level) << "]\t" << func << " @ " << file << ": " << line << ": "
              << oss.str() << '\n';
}

// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define LOG_DEBUG(...) logMessage(LogLevel::DEBUG, __func__, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_INFO(...) logMessage(LogLevel::INFO, __func__, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_WARNING(...) logMessage(LogLevel::WARNING, __func__, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_ERROR(...) logMessage(LogLevel::ERROR, __func__, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_FATAL(...) logMessage(LogLevel::FATAL, __func__, __FILE__, __LINE__, ##__VA_ARGS__);
