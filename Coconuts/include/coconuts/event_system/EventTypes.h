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

#define EVENT_NAME_KEY_PRESS        "Event <KeyPress>"
#define EVENT_NAME_KEY_RELEASE      "Event <KeyRelease>"

/* Macro to write the Getters() for EventType and EventCategories */
#define SET_EVENT_TYPE(TYPE)        EventType GetEventType(void) const override { return TYPE; }
#define SET_EVENT_CATEGORIES(CATS)  unsigned int GetEventCategoryFlags(void) const override { return CATS;}

namespace Coconuts
{
    enum class EventType
    {
        None = 0,           
        WindowResize,
        WindowClose,
        KeyPress,
        KeyRelease,
        MousePress,
        MouseRelease,
        TouchPress,
        TouchRelease
    };
    
    enum EventCategory
    {
        None = 0,
        Category_WindowEvent        = BIT_MASK(1),
        Category_InputKeyEvent      = BIT_MASK(2),  /* Keyboard + Gamepad */
        Category_InputMouseEvent    = BIT_MASK(3),  /* Mouse pointer + Mouse keys */
        Category_InputTouchEvent    = BIT_MASK(4)   /* RESERVED - Future usage */
    };
    
#define IS_INPUT_EVENT(CATS)    ((CATS & Category_InputKeyEvent) || (CATS & Category_InputMouseEvent) || (CATS & Category_InputTouchEvent)) ? true:false
#define IS_WINDOW_EVENT(CATS)   (CATS & Category_WindowEvent) ? true:false
}

#endif /* EVENTTYPES_H */

