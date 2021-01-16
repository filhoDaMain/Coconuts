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
 
    bool SceneOverview::Init(GameLayer*& gameLayer, ComponentInspector* componentInspector)
    {
        m_GameLayerPtr = gameLayer;
        m_ComponentInspectorPtr = componentInspector;
        m_CurrentSelectedEntityPtr = new Coconuts::Entity();
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
            /* Draw all entities */
            if (DrawNode(thisEntity))
            {
                /**
                 * thisEntity is currently selected.
                 * Update Component Inspector panel context to draw its
                 * components.
                 */ 
                /* Update current Entity Ptr */
                *m_CurrentSelectedEntityPtr = thisEntity;
                m_ComponentInspectorPtr->ChangeContext(m_CurrentSelectedEntityPtr);
            }

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
    
    bool SceneOverview::DrawNode(Entity& entity)
    {
        std::string tag = entity.GetComponent<TagComponent>().tag;
        	
        static uint64_t context_id = 0;
        bool hasCameraComponent = false;
        bool hasTransformComponent = false;
        bool hasSpriteComponent = false;
        bool hasBehaviorComponent = false;
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= context_id == entity.GetId() ? ImGuiTreeNodeFlags_Selected : 0;
        
        bool open = ImGui::TreeNodeEx(tag.c_str() , flags);
        
        if (ImGui::IsItemClicked())
        {
            context_id = entity.GetId();
        }
                
        /* Draw simple components */
        if (open)
        {
            ImGui::TextDisabled("ID: %llu", entity.GetId());
            
            /* Camera */
            if (entity.HasComponent<OrthoCameraComponent>())
            {
                ImGui::Text("Camera");
            }
            
            /* Transform */
            if (entity.HasComponent<TransformComponent>())
            {
                ImGui::Text("Transform");
            }
            
            /* Sprite */
            if (entity.HasComponent<SpriteComponent>())
            {
                ImGui::Text("Sprite");
            }
            
            /* Behavior */
            if (entity.HasComponent<BehaviorComponent>())
            {
                ImGui::Text("Behavior");
            }
            
            ImGui::TreePop();
        }
        
        /**
         * This entity is currently selected.
         */
        if (flags & ImGuiTreeNodeFlags_Selected)
        {   
            return true;
        }
        
        return false;
    }
    
}
}