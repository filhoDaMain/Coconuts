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
        class AppWindowResize : public Event
        {
        public:
            AppWindowResize(unsigned int w, unsigned int h)
                :   m_Width(w), m_Height(h) {}
                
            unsigned int GetWidth() const { return m_Width; }
            unsigned int GetHeight() const { return m_Height; }
            
            EventType GetEventType() const override { return EventType::EVTYPE_APP_WINDOW_RESIZE; }
            
            unsigned int GetEventCategoryFlags() const override
            {
                return (EVCATG_WINDOW_CHANGE | EVCATG_SANDBOX_APP);
            }
            
        private:
            unsigned int m_Width, m_Height;
        };
    }
}

#endif /* APPWINDOWEVENT_H */

