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
#ifndef MAINAPP_H
#define MAINAPP_H

#include <coconuts/Application.h>
#include "GameLayer.h"
#include "EditorLayer.h"

namespace Coconuts
{
    
    class MainApp : public Application
    {
    public:
        MainApp();
        ~MainApp() = default;
        
    private:
        GameLayer*      m_GameLayerPtr;
        EditorLayer*    m_EditorLayerPtr;
    };

}

#endif /* MAINAPP_H */

