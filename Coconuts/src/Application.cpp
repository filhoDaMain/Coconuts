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

#include <coconuts/core.h>
#include <coconuts/Logger.h>
#include <coconuts/EventSystem.h>

namespace Coconuts
{
    Application::Application()
    {
        LOG_INFO("Sandbox App created!");
    }
    
    Application::~Application()
    {
        
    }
    
    void Application::Run()
    {
        LOG_INFO("Sandbox App is now running...");
        
        LOG_TRACE("Emulating an AppWindowResize Event");
        Events::AppWindowResize mEvent(10,20);
        
        if (mEvent.IsInCategory(EventCategory::EVCATG_SANDBOX_APP))
        {
            LOG_TRACE("It's a Sandbox App Event");
        }
        
        if (mEvent.IsInCategory(EventCategory::EVCATG_WINDOW_CHANGE))
        {
            LOG_TRACE("It's a Window change Event");
        }
        
        LOG_TRACE("Window size: w: {}  h: {}", mEvent.GetWidth(), mEvent.GetHeight());
        
        while(true);
    }
}