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
            //DrawNode(thisEntity);
            DrawNodeOnComponentInspector(thisEntity);
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
                    m_ComponentInspectorPtr->ChangeContext(
                        &(entity.GetComponent<OrthoCameraComponent>()));
                }
            }
            
            /* Transform? */
            if (entity.HasComponent<TransformComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Transform"))
                {
                    m_ComponentInspectorPtr->ChangeContext(
                        &(entity.GetComponent<TransformComponent>()));
                }
            }
            
            /* Sprite? */
            if (entity.HasComponent<SpriteComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Sprite"))
                {
                    m_ComponentInspectorPtr->ChangeContext(
                        &(entity.GetComponent<SpriteComponent>()));
                }
            }
            
            /* Behavior? */
            if (entity.HasComponent<BehaviorComponent>())
            {
                ImGui::Text("Component:");
                ImGui::SameLine();
                if (ImGui::SmallButton("Behavior"))
                {
                    m_ComponentInspectorPtr->ChangeContext(
                        &(entity.GetComponent<BehaviorComponent>()));
                }
            }
            
            ImGui::TreePop();
        }
    }
    
    void SceneOverview::DrawNodeOnComponentInspector(Entity& entity)
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
        
        /* Query All Components */
        if (entity.HasComponent<OrthoCameraComponent>())
        {
            hasCameraComponent = true;
        }
        
        if (entity.HasComponent<TransformComponent>())
        {
            hasTransformComponent = true;
        }
        
        if (entity.HasComponent<SpriteComponent>())
        {
            hasSpriteComponent = true;
        }
        
        if (entity.HasComponent<BehaviorComponent>())
        {
            hasBehaviorComponent = true;
        }
        
        /* Draw simple components */
        if (open)
        {
            ImGui::TextDisabled("ID: %llu", entity.GetId());
            
            if (hasCameraComponent)
            {
                ImGui::Text("Camera");
            }
            
            if (hasTransformComponent)
            {
                ImGui::Text("Transform");
            }
            
            if (hasSpriteComponent)
            {
                ImGui::Text("Sprite");
            }
            
            if (hasBehaviorComponent)
            {
                ImGui::Text("Behavior");
            }
            
            ImGui::TreePop();
        }
        
        /**
         * This entity is currently selected.
         * Draw its components on Component Inspector Panel.
         * 
         */
        if (flags & ImGuiTreeNodeFlags_Selected)
        {   
            if (hasCameraComponent)
            {
                m_ComponentInspectorPtr->ChangeContext(
                    &(entity.GetComponent<OrthoCameraComponent>()));
            }
            
            if (hasTransformComponent)
            {
                m_ComponentInspectorPtr->ChangeContext(
                    &(entity.GetComponent<TransformComponent>()));
            }
            
            if (hasSpriteComponent)
            {
                m_ComponentInspectorPtr->ChangeContext(
                    &(entity.GetComponent<SpriteComponent>()));
            }
            
            if (hasBehaviorComponent)
            {
                m_ComponentInspectorPtr->ChangeContext(
                    &(entity.GetComponent<BehaviorComponent>()));
            }
        }
        
        //if (ImGui::TreeNode((void*)entity.GetId(), flags, tag.c_str()))
        //{
        //    
        //    ImGui::TreePop();
        //}
    }
    
}
}