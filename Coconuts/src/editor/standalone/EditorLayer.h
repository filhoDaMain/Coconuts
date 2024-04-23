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
#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <coconuts/window_system/Window.h>
#include <coconuts/editor.h>
#include <coconuts/graphics/Renderer2D.h>
#include <coconuts/layer_system/GameLayer.h>
#include <glm/glm.hpp>

/* Menu Bar */
#include "menu_bar/FileMenu.h"
#include "menu_bar/EntityMenu.h"
#include "menu_bar/AssetsMenu.h"

/* Panels */
#include "panels/Viewport.h"
#include "panels/Statistics.h"
#include "panels/SceneOverview.h"
#include "panels/ComponentInspector.h"
#include "panels/Assets.h"
#include "panels/AssetInspector.h"

/* PopUps */
#include "popups/ImportTexture2DPopUp.h"
#include "popups/CreateSpritePopUp.h"
#include "popups/LoadProjectPopUp.h"
#include "popups/SaveProjectPopUp.h"

namespace Coconuts
{
    
    class EditorLayer
    {
    public:
        EditorLayer(const Window& windowContext) : m_Window(windowContext), m_GameLayerPtr() {}
        ~EditorLayer() = default;
  
        void Init(void);
        void Draw();

    private:
        void PreInit(void);
        void PostInit(void);

        // Draw Calls
        void Begin(void);
        void RenderGUI(void);
        void End();
        void SetCustomGUIStyle();
        void SetDarkThemeColors();
        void LoadLayout();
        void SetDefaultFontTTF(const std::string& pathToFileTTF, float size);
        bool LoadLayoutFile(const std::string& file);

    private:
        // Window context
        const Window& m_Window;

        /* Show PopUp flags */
        bool m_ShowPopUp_CreateSprite;
        bool m_ShowPopUp_ImportTexture2D;
        bool m_ShowPopUp_LoadProject;
        bool m_ShowPopUp_SaveProject;

        GameLayer* m_GameLayerPtr;

        /* Menu Bar menus */
        MenuBar::FileMenu               m_FileMenu;
        MenuBar::EntityMenu             m_EntityMenu;
        MenuBar::AssetsMenu             m_AssetsMenu;

        /* Panels */
        Panels::Viewport                m_ViewportPanel;
        Panels::Statistics              m_StatisticsPanel;
        Panels::SceneOverview           m_SceneOverviewPanel;
        Panels::ComponentInspector      m_ComponentInspectorPanel;
        Panels::Assets                  m_AssetsPanel;
        Panels::AssetInspector          m_AssetInspectorPanel;

        /* PopUps */
        PopUps::ImportTexture2DPopUp    m_ImportTexture2dPopUp;
        PopUps::CreateSpritePopUp       m_CreateSpritePopUp;
        PopUps::LoadProjectPopUp        m_LoadProjectPopUp;
        PopUps::SaveProjectPopUp        m_SaveProjectPopUp;
    };
    
}
#endif /* EDITORLAYER_H */

