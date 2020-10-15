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

#include <coconuts/EventSystem.h>
#include <coconuts/Application.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
    /* Dispatch to -> Application::Method */
    template <>
    bool EventDispatcher::StaticDispatch<Application>(Event& event, Application* object)
    {
        unsigned int event_categories = event.GetEventCategoryFlags();
        
        /* Input Event */
        if (IS_INPUT_EVENT(event_categories))
        {
            //TODO
            return true;
        }
        
        /* Window Event */
        if (IS_WINDOW_EVENT(event_categories))
        {
            switch(event.GetEventType())
            {
                case EventType::None:
                {
                    return false;
                    break;
                }
                
                case EventType::WindowResize:
                {
                    return false;
                    break;
                }
                
                case EventType::WindowClose:
                {
                    return false;
                    break;
                }
                
                default:
                    return false;
            }
        }
        
        return false;
    }
    
    
    
}