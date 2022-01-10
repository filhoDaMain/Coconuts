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

#include <coconuts/debug/TimeProfiler.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    //private
    Profiler::TimeProfiler::TimeProfiler()
    : m_Profiles(), m_Guard()
    {
        // do nothing
    }
    
    Profiler::TimeProfiler& Profiler::TimeProfiler::GetInstance()
    {
        static TimeProfiler s_Instance;
        return s_Instance;
    }
    
    void Profiler::TimeProfiler::Push(TimeData data)
    {
        std::lock_guard<std::mutex> lock(m_Guard);
        const auto found = m_Profiles.find(data.scopeName);
        
        /**
         * First time pushing data for the given map key.
         * Reserve vector space once to avoid further re-allocations.
         */
        if (found == m_Profiles.end())
        {
            /* Allocation of a map element */
            m_Profiles[data.scopeName];
            
            /* Allocation of its vector */
            m_Profiles[data.scopeName].reserve(Profiler::Constants::TIMEPROFILER_BUFFER_SIZE);
        }
        
        /* Get vector for the given key */
        auto& vec = m_Profiles[data.scopeName];
        
        /**
         * Buffer reached its maximum pre-defined max size.
         * Discard oldest element to avoid a re-allocation.
         */
        if (vec.size() == Profiler::Constants::TIMEPROFILER_BUFFER_SIZE)
        {
            vec.erase(vec.cbegin());
        }
        
        /* Push element */
        vec.push_back(data);
    }
    
    std::vector<Profiler::TimeData> Profiler::TimeProfiler::Fetch(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_Guard);
        
        auto found = m_Profiles.find(key);
        
        if (found != m_Profiles.end())
        {
            return found->second;
        }
        
        // ELSE: Not found -> return dummy object
        TimeData dummy = {"not found", "null", "null", 0, 0};
        return std::vector<Profiler::TimeData> {dummy};
    }
    
    bool Profiler::TimeProfiler::Clear(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_Guard);
        
        const auto& found = m_Profiles.find(key);
        
        if (found != m_Profiles.end())
        {
            found->second.clear();
            return true;
        }
        
        return false;
    }
    
}