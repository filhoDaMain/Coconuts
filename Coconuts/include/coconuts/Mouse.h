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
#ifndef MOUSE_H
#define MOUSE_H

/* Codes taken from glfw3.h */

namespace
{
    
    enum Mouse
    {
        MOUSE_BUTTON_1      =   0,
        MOUSE_BUTTON_2      =   1,
        MOUSE_BUTTON_3      =   2,
        MOUSE_BUTTON_4      =   3,
        MOUSE_BUTTON_5      =   4,
        MOUSE_BUTTON_6      =   5,
        MOUSE_BUTTON_7      =   6,
        MOUSE_BUTTON_8      =   7,
        MOUSE_BUTTON_LAST   =   Mouse::MOUSE_BUTTON_8,
        MOUSE_BUTTON_LEFT   =   Mouse::MOUSE_BUTTON_1,
        MOUSE_BUTTON_RIGHT  =   Mouse::MOUSE_BUTTON_2,
        MOUSE_BUTTON_MIDDLE =   Mouse::MOUSE_BUTTON_3
    };
    
}

#endif /* MOUSE_H */

