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
#include <cstdint>

namespace Coconuts
{
    namespace WindowEvent
    {
        class WindowResize : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_WindowEvent);
            SET_EVENT_TYPE(EventType::WindowResize);
            
            WindowResize(unsigned int w, unsigned int h)
                :   m_Width(w), m_Height(h) {}
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_RESIZE << " (" << m_Width << ", " << m_Height << ")";
                return ss.str();
            }
                   
            float GetWidth() const { return m_Width; }
            float GetHeight() const { return m_Height; }
            
        private:
            float m_Width, m_Height;
        };
        
        
        class WindowClose : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_WindowEvent);
            SET_EVENT_TYPE(EventType::WindowClose);
            
            WindowClose() = default;
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_CLOSE;
                return ss.str();
            }
        };
        
        class WindowMinimize : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_WindowEvent);
            SET_EVENT_TYPE(EventType::WindowMinimize);
            
            WindowMinimize(bool isMinimized)
                :   m_isMinimized(isMinimized) {}
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_MINIMIZE;
                return ss.str();
            }
            
            bool IsMinimized() const { return m_isMinimized; }
            
        private:
            bool m_isMinimized = false;
        };
    }
}

#endif /* WINDOWEVENT_H */

