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
        m_Profiles[data.scopeName] = data;
    }
    
    std::vector<Profiler::TimeData> Profiler::TimeProfiler::FetchAll()
    {
        std::lock_guard<std::mutex> lock(m_Guard);
        std::vector<TimeData> tmp;
        std::map<std::string, TimeData>::iterator it;
        
        for (it = m_Profiles.begin(); it != m_Profiles.end(); it++)
        {
            tmp.push_back(it->second);
        }
        
        return tmp;
    }
    
    Profiler::TimeData Profiler::TimeProfiler::Fetch(std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_Guard);
        auto found = m_Profiles.find(key);
        
        if (found != m_Profiles.end())
        {
            return found->second;
        }
        
        // ELSE: Not found -> return dummy object
        TimeData dummy = {"not found", "null", "null", 0, 0};
        return dummy;
    }
    
}