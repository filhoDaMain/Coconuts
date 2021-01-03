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

#include "SceneOverview.h"
#include <coconuts/editor.h>
#include <string>
#include <coconuts/Logger.h>

// ECS Components
#include <coconuts/ecs/components/TagComponent.h>
#include <coconuts/ecs/components/OrthoCameraComponent.h>
#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/SpriteComponent.h>
#include <coconuts/ecs/components/BehaviorComponent.h>

namespace Coconuts {
namespace Panels
{
 
    bool SceneOverview::Init(GameLayer*& gameLayer)
    {
        m_GameLayerPtr = gameLayer;
        
        return true;
    }
    
    void SceneOverview::Draw()
    {
        if ( m_GameLayerPtr->IsActiveSceneUpdated() )
        {
            GetLastSceneUpdate();
        }
        
        ImGui::Begin("Scene Overview");
        for (Entity thisEntity : sceneEntities)
        {
            // List all Tag Component
            DrawNode(thisEntity);
            ImGui::Spacing();
        }
        ImGui::End();
    }
    
    void SceneOverview::GetLastSceneUpdate()
    {
        /* Get tmp vector from Game Layer with al entities */
        std::vector<Entity> last = m_GameLayerPtr->GetActiveSceneEntities();
        
        /* Free heap vector and re-allocate */
        sceneEntities = std::vector<Entity>();
        sceneEntities.resize(last.size());
        
        /* Copy */
        sceneEntities = last;
    }
    
    void SceneOverview::DrawNode(Entity& entity)
    {
        std::string name = entity.GetComponent<TagComponent>().tag;
        
        if (ImGui::TreeNode(name.c_str()))
        {
            ImGui::TextDisabled("Entity ID: %llu", entity.GetId());
            
            /* Camera? */
            if (entity.HasComponent<OrthoCameraComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Orthographic Camera"))
                {
                }
            }
            
            /* Transform? */
            if (entity.HasComponent<TransformComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Transform"))
                {
                }
            }
            
            /* Sprite? */
            if (entity.HasComponent<SpriteComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Sprite"))
                {
                }
            }
            
            /* Behavior? */
            if (entity.HasComponent<BehaviorComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Behavior"))
                {
                }
            }
            
            ImGui::TreePop();
        }
    }
    
}
}