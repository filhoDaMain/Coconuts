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

#include "FileMenu.h"
#include <coconuts/editor.h>
#include <coconuts/Logger.h>
#include <coconuts/Application.h>
#include <coconuts/EventSystem.h>

namespace Coconuts {
namespace MenuBar
{
 
    bool FileMenu::Init(GameLayer*& gameLayer, bool* showPopUpLoadProj, bool* showPopUpSaveProj)
    {
        m_GameLayerPtr = gameLayer;
        m_ShowPopUpLoadProj = showPopUpLoadProj;
        m_ShowPopUpSaveProj = showPopUpSaveProj;
        return true;
    }
    
    void FileMenu::Draw()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Close", "", false, true))
            {
                Close();
            }
            
            if (ImGui::MenuItem("Open Project", "", false, true))
            {
                OpenProject();
            }
            
            if (ImGui::MenuItem("Svae Project As...", "", false, true))
            {
                SaveProjectAs();
            }
            
            ImGui::EndMenu();
        }
    }
    
    void FileMenu::Close()
    {
        LOG_TRACE("File Menu: Close");
        
        /**
         * Create a WindowClose Event which will be propogated
         * and handled by the assigned handler.
         */
        WindowEvent::WindowClose winCloseEvent;
        Application::GetInstance().GetWindow().EventTrigger(winCloseEvent);
    }
    
    void FileMenu::OpenProject()
    {
        LOG_TRACE("File Menu: Open Project");
        *m_ShowPopUpLoadProj = true;
    }
    
    void FileMenu::SaveProjectAs()
    {
        LOG_TRACE("File Menu: Save Project As");
        *m_ShowPopUpSaveProj = true;
    }
    
}
}