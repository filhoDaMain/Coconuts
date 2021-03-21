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

#include <coconuts/editor.h>
#include <coconuts/graphics/Renderer2D.h>
#include "GameLayer.h"
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

namespace Coconuts
{
    
    class EditorLayer : public Editor::GUILayer
    {
    public:
        EditorLayer() = default;
        EditorLayer(GameLayer* gameLayer) : m_GameLayerPtr(gameLayer) {}
        virtual ~EditorLayer() = default;
  
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnPostAttach() override;
        
    private:
        void SetCustomGUIStyle();
        void SetDarkThemeColors();
        
    private:
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
    };
    
}
#endif /* EDITORLAYER_H */

