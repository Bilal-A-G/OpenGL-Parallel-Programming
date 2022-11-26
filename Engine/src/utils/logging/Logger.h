#pragma once
#include "spdlog/spdlog.h"
#include"spdlog/sinks/stdout_color_sinks.h"

namespace TESLA
{
    class Logger
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger> GetEngineLogger() {return m_engineLogger;}
        static std::shared_ptr<spdlog::logger> GetClientLogger(){return m_clientLogger;}
    private:
        static std::shared_ptr<spdlog::logger> m_engineLogger;
        static std::shared_ptr<spdlog::logger> m_clientLogger;
        
    };
}

namespace TESLA_LOGGER
{
    constexpr spdlog::level::level_enum WARN = spdlog::level::warn;
    constexpr spdlog::level::level_enum INFO = spdlog::level::info;
    constexpr spdlog::level::level_enum TRACE = spdlog::level::trace;
    constexpr spdlog::level::level_enum DEBUG = spdlog::level::debug;
    constexpr spdlog::level::level_enum CRITICAL = spdlog::level::critical;
    constexpr spdlog::level::level_enum ERR = spdlog::level::err;
}

#ifdef TS_DEBUG
#ifdef TS_ENGINE
#define TS_LOG_MESSAGE(...) ::TESLA::Logger::GetEngineLogger()->log(__VA_ARGS__)
#define TS_LOG_ASSERTION(x, ...){if(!x) {TESLA::Logger::GetEngineLogger()->log(__VA_ARGS__); __debugbreak(); } }
#else
#define TS_LOG_MESSAGE(...) ::TESLA::Logger::GetClientLogger()->log(__VA_ARGS__)
#define TS_LOG_ASSERTION(x, ...){if(!x) {TESLA::Logger::GetClientLogger()->log(__VA_ARGS__); __debugbreak(); } }
#endif
#else
#define TS_LOG_MESSAGE(...)
#define TS_LOG_ASSERTION(x, ...)
#endif
