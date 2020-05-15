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
#ifndef EVENTTYPES_H
#define EVENTTYPES_H

#include <coconuts/types.h>

/* Log Output Event names */
#define EVENT_NAME_WINDOW_RESIZE    "Event <WindowResize>"
#define EVENT_NAME_WINDOW_CLOSE     "Event <WindowClose>"

namespace Coconuts
{
    enum class EventType
    {
        None = 0,           
        WindowResize,
        WindowClose
    };
    
    enum EventCategory
    {
        None = 0,
        WindowChange    = BIT_MASK(1),  /* Related to a window change */
        SandboxApp      = BIT_MASK(2),  /* Related to the sanbox app */
    };
}

#endif /* EVENTTYPES_H */

