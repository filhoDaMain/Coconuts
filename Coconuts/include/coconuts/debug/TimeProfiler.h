/*
 * Copyright 2021 Andre Temprilho
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
#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>

namespace Coconuts
{
    
    namespace Profiler
    {
        enum Constants
        {
            TIMEPROFILER_BUFFER_SIZE = 128U
        };
        
        struct TimeData
        {
            std::string scopeName;
            std::string file;
            std::string function;
            uint32_t    line;
            uint32_t    deltaTime_us;   // in microseconds
        };
    
        
        class TimeProfiler
        {
            friend class InstrumentationTimer;  // forward declared (below)
            
        public:
            ~TimeProfiler() = default;
        
            /**
             * Singleton implementation.
             */
            TimeProfiler(TimeProfiler const&) = delete;
            void operator = (TimeProfiler const&) = delete;
        
            static TimeProfiler& GetInstance();
            std::vector<TimeData> Fetch(const std::string& key);
            bool Clear(const std::string& key);
            
        private:
            TimeProfiler(); // singleton
            void Push(TimeData data);
            
        private:
            std::map<std::string, std::vector<TimeData>> m_Profiles;
            std::mutex m_Guard;
        };
        
        
        class InstrumentationTimer
        {
        public:
            InstrumentationTimer(const char* scope, const char* file, const char* func, uint32_t line)
            : m_Data(),
              m_Scope(scope),
              m_File(file),
              m_Function(func),
              m_Line(line),
              m_StartTimepoint(),
              m_StopTimepoint()
            {
                Start();
            }
            
            ~InstrumentationTimer()
            {
                Stop();
            }
            
        private:
            void Start()
            {
                m_StartTimepoint = std::chrono::high_resolution_clock::now();
            }
        
            void Stop()
            {
                m_StopTimepoint = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_StopTimepoint - m_StartTimepoint);
                
                m_Data.scopeName        = m_Scope;
                m_Data.file             = m_File;
                m_Data.function         = m_Function;
                m_Data.line             = m_Line;
                m_Data.deltaTime_us     = static_cast<uint32_t> (duration.count());
                
                // Publish results
                TimeProfiler::GetInstance().Push(m_Data);
            }
            
        private:
            Profiler::TimeData m_Data;
            std::string m_Scope;
            std::string m_File;
            std::string m_Function;
            uint32_t m_Line;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StopTimepoint;
        };
        
    }   // namespace Profiler
    
}   // namespace Coconuts

#endif /* TIMEPROFILER_H */

