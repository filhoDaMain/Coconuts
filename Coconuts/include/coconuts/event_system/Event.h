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

#include <coconuts/types.h>

namespace Coconuts
{
    
    enum class EventType
    {
        EVTYPE_NONE = 0,           
        EVTYPE_APP_WINDOW_RESIZE    
    };
    
    enum EventCategory
    {
        EVCATG_NONE = 0,
        EVCATG_WINDOW_CHANGE    = BIT_MASK(1),  /* Related a window change */
        EVCATG_SANDBOX_APP      = BIT_MASK(2),  /* Related to the sanbox app */
    };
    
    class Event
    {
    public:
        virtual EventType GetEventType() const = 0;
        virtual unsigned int GetEventCategoryFlags() const = 0;
        
        bool IsInCategory(EventCategory cat)
        {
            return ( GetEventCategoryFlags() & cat );
        }
        
    private:
    };
}


#endif /* EVENT_H */

