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
#ifndef EVENT_H
#define EVENT_H

#include <coconuts/event_system/EventTypes.h>
#include <string>
#include <sstream>

namespace Coconuts
{   
    class Event
    {
    public:
        bool handled = false;
        
        virtual std::string ToString(void) const = 0;
        virtual EventType GetEventType(void) const = 0;
        virtual unsigned int GetEventCategoryFlags(void) const = 0;
        
        bool IsInCategory(EventCategory cat)
        {
            return ( GetEventCategoryFlags() & cat );
        }
    };
}


#endif /* EVENT_H */

