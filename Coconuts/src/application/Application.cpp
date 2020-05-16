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

#include <coconuts/application/Application.h>
#include <coconuts/core.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Logger.h>
#include <functional>


namespace Coconuts
{
    Application::Application()
    {
        LOG_DEBUG("Sandbox App created!");
        p_Window = std::unique_ptr<Window>(Window::Create());
        
#if defined(__APPLE__)
        if (p_Window != nullptr)
        {
            LOG_DEBUG("Coconuts WindowSystem initialized for the MacOS Platform");
        }
        //ELSE - it should already have crashed!
#elif __gnu_linux__
        if (p_Window != nullptr)
        {
            LOG_DEBUG("Coconuts WindowSystem initialized for the GNU Platform");
        }
        //ELSE - it should already have crashed!
#endif
        
        /* Whenever an event occurs inside p_Window OnEvent gets called */
        p_Window->SetEventCallback( std::bind(&Application::OnEvent, this, std::placeholders::_1) );
        
    }
    
    Application::~Application()
    {

    }
    
    void Application::Run()
    {
        m_isRunning = true;
        LOG_INFO("Sandbox App is now running...");
        
        while(m_isRunning)
        {
            p_Window->OnUpdate();
        }
    }
    
    void Application::OnEvent(Event& event)
    {
        bool handled = false;
        
        /* Log Event */
        LOG_TRACE(event.ToString());
        
        /* Dispatch Event */
        handled = EventDispatcher::StaticDispatch<Application>(event, this);
        
        if (!handled)
        {
            LOG_WARN("Event could not be dispatched by the EventDispatcher");
        }
    }
    
    void Application::OnWindowClose()
    {
        LOG_WARN("Performing a graceful shutdown sequence...");
        m_isRunning = false;
    }
}