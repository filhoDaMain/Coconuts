/*
 * Copyright 2020 Andre Temprilho
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Coconuts
{
    class Logger
    {
    public:
        static void Init();
        
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
        
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#ifdef __COCONUTS_SANDBOX_APP__
#define LOG_TRACE(...)      Coconuts::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)      Coconuts::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)       Coconuts::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       Coconuts::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      Coconuts::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)   Coconuts::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
#define LOG_TRACE(...)      Coconuts::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)      Coconuts::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)       Coconuts::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       Coconuts::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      Coconuts::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)   Coconuts::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#endif

#endif /* LOGGER_H */

