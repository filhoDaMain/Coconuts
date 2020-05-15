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
#ifndef APPWINDOWEVENT_H
#define APPWINDOWEVENT_H

#include <coconuts/event_system/Event.h>

namespace Coconuts
{
    namespace Events
    {
        class WindowResize : public Event
        {
        public:
            WindowResize(unsigned int w, unsigned int h)
                :   m_Width(w), m_Height(h) {}
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_WINDOW_RESIZE << " (" << m_Width << ", " << m_Height << ")";
                return ss.str();
            }
                
            EventType GetEventType(void) const override { return EventType::EVTYPE_WINDOW_RESIZE; }
            
            unsigned int GetEventCategoryFlags(void) const override
            {
                return (EVCATG_WINDOW_CHANGE | EVCATG_SANDBOX_APP);
            }
                   
            unsigned int GetWidth() const { return m_Width; }
            unsigned int GetHeight() const { return m_Height; }
            
        private:
            unsigned int m_Width, m_Height;
        };
    }
}

#endif /* APPWINDOWEVENT_H */

