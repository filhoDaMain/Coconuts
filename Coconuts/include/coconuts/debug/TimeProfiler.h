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

#include <iostream>

namespace Coconuts
{
    
    namespace Profiler
    {
        struct TimeData
        {
            std::string scopeName;
            uint32_t    executionTime_us;   // in microseconds
        };
    }
    
    class TimeProfiler
    {
    public:
        ~TimeProfiler() = default;
        
        /**
         * Singleton implementation.
         */
        TimeProfiler(TimeProfiler const&) = delete;
        void operator = (TimeProfiler const&) = delete;
        
        static TimeProfiler& GetInstance()
        {
            static TimeProfiler s_Instance;
            return s_Instance;
        }
        
        void Push(Profiler::TimeData data);
        std::vector<Profiler::TimeData> Fetch();
        
        
    private:
        TimeProfiler(); // singleton
        
    private:
        std::map<std::string, Profiler::TimeData> m_Profiles;
    };
    
    
    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* scope)
        : m_Data(), m_Scope(scope), m_StartTimepoint(), m_StopTimepoint()
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
            
            m_Data.scopeName = m_Scope;
            m_Data.executionTime_us = static_cast<uint32_t> (duration.count());
            
            // Publish results
            TimeProfiler::GetInstance().Push(m_Data);
        }
        
    private:
        Profiler::TimeData m_Data;
        std::string m_Scope;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StopTimepoint;
    };
    
}

#endif /* TIMEPROFILER_H */

