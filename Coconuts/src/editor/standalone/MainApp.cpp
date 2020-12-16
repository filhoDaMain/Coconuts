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

#include "MainApp.h"

namespace Coconuts
{
    
    MainApp::MainApp()
    : Application("Coconuts Editor")
    {
        m_GameLayerPtr      = new GameLayer();
        m_EditorLayerPtr    = new EditorLayer(m_GameLayerPtr);
        
        PushLayer(m_GameLayerPtr);          // In-Game graphics
        PushOverlay(m_EditorLayerPtr);      // Editor UI graphics
    }
    
}