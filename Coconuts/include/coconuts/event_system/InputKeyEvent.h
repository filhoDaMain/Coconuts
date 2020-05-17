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
#ifndef INPUTKEYEVENT_H
#define INPUTKEYEVENT_H

#include <coconuts/event_system/EventTypes.h>
#include <coconuts/event_system/Event.h>

namespace Coconuts
{
    namespace InputKeyEvent
    {
        class KeyPress : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputKeyEvent);
            SET_EVENT_TYPE(EventType::KeyPress);
            
            KeyPress(int keycode, unsigned int repeatCount)
                : m_KeyCode(keycode), m_RepeatCount(repeatCount)
            {}
                
            inline int GetKeyCode(void) const { return m_KeyCode; }
            inline unsigned int GetRepeatCount(void) const { return m_RepeatCount; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_KEY_PRESS << " [keycode = " << m_KeyCode << "; repeat = " << m_RepeatCount << "]";
                return ss.str();
            }
            
        private:
            int             m_KeyCode;
            unsigned int    m_RepeatCount;
        };
        
        
        class KeyRelease : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputKeyEvent);
            SET_EVENT_TYPE(EventType::KeyRelease);
            
            KeyRelease(int keycode) : m_KeyCode(keycode) {}
            
            inline int GetKeyCode(void) const { return m_KeyCode; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_KEY_RELEASE << " [keycode = " << m_KeyCode <<"]";
                return ss.str();
            }
            
        private:
            int             m_KeyCode;
        };
    }
}

#endif /* INPUTKEYEVENT_H */

