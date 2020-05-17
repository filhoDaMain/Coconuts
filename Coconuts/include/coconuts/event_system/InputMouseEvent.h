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
#ifndef INPUTMOUSEEVENT_H
#define INPUTMOUSEEVENT_H

#include <coconuts/event_system/EventTypes.h>
#include <coconuts/event_system/Event.h>

namespace Coconuts
{
    namespace InputMouseEvent
    {
        class MouseButtonPress : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputMouseEvent);
            SET_EVENT_TYPE(EventType::MouseButtonPress);
            
            MouseButtonPress(int buttoncode) : m_ButtonCode(buttoncode) {}
            
            inline int GetButtonCode(void) const { return m_ButtonCode; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_MOUSE_BUTTON_PRESS << " [buttoncode = " << m_ButtonCode << "]";
                return ss.str();
            }
            
        private:
            int m_ButtonCode;
        };
        
        
        class MouseButtonRelease : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputMouseEvent);
            SET_EVENT_TYPE(EventType::MouseButtonRelease);
            
            MouseButtonRelease(int buttoncode) : m_ButtonCode(buttoncode) {}
            
            inline int GetButtonCode(void) const { return m_ButtonCode; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_MOUSE_BUTTON_RELEASE << " [buttoncode = " << m_ButtonCode << "]";
                return ss.str();
            }
            
        private:
            int m_ButtonCode;
        };
        
        
        class MouseCursorMove : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputMouseEvent);
            SET_EVENT_TYPE(EventType::MouseCursorMove);
            
            MouseCursorMove(double x, double y)
                : m_MouseX(x), m_MouseY(y)
            {}
            
            inline double GetX(void) const { return m_MouseX; }
            inline double GetY(void) const { return m_MouseY; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_MOUSE_CURSOR_MOVE << " [(" << m_MouseX <<";" << m_MouseY << ")]";
                return ss.str();
            }
            
        private:
            double m_MouseX, m_MouseY;
        };
        
        
        class MouseScroll : public Event
        {
        public:
            SET_EVENT_CATEGORIES(Category_InputMouseEvent);
            SET_EVENT_TYPE(EventType::MouseScroll);
            
            MouseScroll(double offX, double offY)
                : m_OffsetX(offX), m_OffsetY(offY)
            {}
            
            inline double GetOffsetX(void) const { return m_OffsetX; }
            inline double GetOffsetY(void) const { return m_OffsetY; }
            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << EVENT_NAME_MOUSE_SCROLL << " [(" << m_OffsetX <<";" << m_OffsetY << ")]";
                return ss.str();
            }
            
        private:
            double m_OffsetX, m_OffsetY;
        };
    }
}

#endif /* INPUTMOUSEEVENT_H */

