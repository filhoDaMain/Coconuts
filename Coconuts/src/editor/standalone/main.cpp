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

/* Standalone Editor entry point */
#ifndef __COCONUTS_STANDALONE_EDITOR_APP__
#define __COCONUTS_STANDALONE_EDITOR_APP__
#endif  // __COCONUTS_STANDALONE_EDITOR_APP__

#include <coconuts/core.h>
#include "MainApp.h"

namespace Coconuts
{
    
    Application* CreateApplication()
    {
        return new MainApp();
    }

}

