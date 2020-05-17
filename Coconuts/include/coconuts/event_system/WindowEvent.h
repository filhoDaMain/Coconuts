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
#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include <coconuts/event_system/EventTypes.h>
#include <coconuts/event_system/Event.h>

namespace Coconuts
{
    namespace WindowEvent
    {
        class WindowResize : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_WindowEvent|Category_SandboxApp);
            SET_EVENT_TYPE(EventType::WindowResize);
            
            WindowResize(unsigned int w, unsigned int h)
                :   m_Width(w), m_Height(h) {}
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_RESIZE << " (" << m_Width << ", " << m_Height << ")";
                return ss.str();
            }
                
#if 0
            EventType GetEventType(void) const override { return EventType::WindowResize; }
            
            unsigned int GetEventCategoryFlags(void) const override
            {
                return (Category_WindowEvent | Category_SandboxApp);
            }
#endif
                   
            unsigned int GetWidth() const { return m_Width; }
            unsigned int GetHeight() const { return m_Height; }
            
        private:
            unsigned int m_Width, m_Height;
        };
        
        
        class WindowClose : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_WindowEvent|Category_SandboxApp);
            SET_EVENT_TYPE(EventType::WindowClose);
            
            WindowClose() = default;
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_CLOSE;
                return ss.str();
            }
             
#if 0
            EventType GetEventType(void) const override { return EventType::WindowClose; }
            
            unsigned int GetEventCategoryFlags(void) const override
            {
                return (Category_WindowEvent | Category_SandboxApp);
            }
#endif
        };
    }
}

#endif /* WINDOWEVENT_H */

