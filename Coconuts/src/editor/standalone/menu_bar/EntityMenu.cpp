/*
 * Copyright 2021 Andre Temprilho
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

#include "EntityMenu.h"
#include <coconuts/editor.h>
#include <coconuts/Logger.h>

namespace Coconuts {
namespace MenuBar
{
    
    bool EntityMenu::Init(GameLayer*& gameLayer)
    {
        m_GameLayerPtr = gameLayer;
        return true;
    }
    
    void EntityMenu::Draw()
    {
        if (ImGui::BeginMenu("Entity"))
        {
            if (ImGui::MenuItem("Create Empty", "", false, true))
            {
                LOG_TRACE("Entity Menu: Create Empty");
                CreateEmpty();
            }
            
            ImGui::EndMenu();
        }
    }
    
    void EntityMenu::CreateEmpty()
    {
        m_GameLayerPtr->NewEntity();
    }
    
}
}