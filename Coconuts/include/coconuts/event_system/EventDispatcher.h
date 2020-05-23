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
#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <coconuts/event_system/Event.h>
#include <coconuts/event_system/EventTypes.h>

namespace Coconuts
{
    class EventDispatcher
    {
    public:
        template <typename T>
        static bool StaticDispatch(Event& event, T* object);
        
        EventDispatcher(Event& event)   : m_Event(event)
        {
        }
        
        template <typename C, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == C::GetStaticType())
            {
                m_Event.handled = func(static_cast<C&>(m_Event));
                return true;
            }
            return false;
        }
        
    private:
        Event& m_Event;
    };
}

#endif /* EVENTDISPATCHER_H */

