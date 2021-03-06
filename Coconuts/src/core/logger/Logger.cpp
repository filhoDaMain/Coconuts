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

#include <coconuts/Logger.h>

namespace Coconuts
{
    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_EditorLogger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
    
    void Logger::Init()
    {        
        spdlog::set_pattern("%d-%m-%Y %H:%M:%S.%e [%-8n] %^%-8l  %v%$");
        
        s_CoreLogger = spdlog::stdout_color_mt  ("ccn-core");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        s_EditorLogger = spdlog::stdout_color_mt  ("ccn-edtr");
        s_EditorLogger->set_level(spdlog::level::trace);
        
        s_ClientLogger = spdlog::stdout_color_mt("APP-GAME");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}