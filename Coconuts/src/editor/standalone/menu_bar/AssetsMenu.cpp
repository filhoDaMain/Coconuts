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

#include "AssetsMenu.h"
#include <coconuts/editor.h>

namespace Coconuts {
namespace MenuBar
{
    
    void AssetsMenu::Draw(bool* showPopUp_ImportTexture2D,
                          bool* showPopUp_CreateSprite)
    {
        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Import Texture2D", "", false, true))
            {
                *showPopUp_ImportTexture2D = true;  // triggers a PopUp handled by EditorLayer
            }
            
            if (ImGui::MenuItem("New Sprite", "", false, true))
            {
                *showPopUp_CreateSprite = true;  // triggers a PopUp handled by EditorLayer
            }
            
            ImGui::EndMenu();
        }
    }
    
}
}