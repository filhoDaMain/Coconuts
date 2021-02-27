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

#include "ComponentInspector.h"
#include <coconuts/editor.h>
#include <string.h>
#include <sstream>
#include <coconuts/Logger.h>
#include "../ed_utils.h"

namespace Coconuts {
namespace Panels
{
     
    bool ComponentInspector::Init()
    {
        return true;
    }
    
    /* Draw */
    void ComponentInspector::Draw()
    {
        ImGui::Begin("Component Inspector");
        
        /* Draw all Components */
        if (hasValidContext)
        {
            /* Tag */
            if (m_Context->HasComponent<TagComponent>())
            {
                DrawTagComponent();
            }
            
            /* Camera */
            if (m_Context->HasComponent<OrthoCameraComponent>())
            {
                hasCameraComponent = true;
                DrawCameraComponent();
            }
            
            /* Transform */
            if (m_Context->HasComponent<TransformComponent>())
            {
                hasTransformComponent = true;
                DrawTransformComponent();
            }
            
            /* Sprite */
            if (m_Context->HasComponent<SpriteComponent>())
            {
                hasSpriteComponent = true;
                DrawSpriteComponent();
            }
            
            /* Behavior */
            if (m_Context->HasComponent<BehaviorComponent>())
            {
                hasBehaviorComponent = true;
                DrawBehaviorComponent();
            }
            
            /* Add Component */
            DrawButtonAddComponent();
        }
        
        else
        {
            DrawEmpty();
        }
        
        /* Reset all flags */
        hasCameraComponent      = false;
        hasTransformComponent   = false;
        hasSpriteComponent      = false;
        hasBehaviorComponent    = false;
        
        ImGui::End();
    }
    
    /* Empty Inspector */
    void ComponentInspector::DrawEmpty(void)
    {
        // such empty :)
    }
    
    
    /* ************************************************************************* */
    /* Component Specific Drawing functions */
    
    /* Tag */
    void ComponentInspector::DrawTagComponent(void)
    {
        TagComponent& tagComponent = m_Context->GetComponent<TagComponent>();

        char buffer[16];
        memset(buffer, 0x00, sizeof(buffer));
        strcpy(buffer, tagComponent.tag.c_str());

        ImGui::Text("Tag");
        if (ImGui::InputText(" ", buffer, sizeof(buffer)))
        {
            tagComponent.tag = std::string(buffer);
        }

        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    /* Camera */
    void ComponentInspector::DrawCameraComponent(void)
    {
        OrthoCameraComponent& cameraComponent = m_Context->GetComponent<OrthoCameraComponent>();
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Camera Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            utils::DrawTableTextButton("Camera Type", "Othographic");
            
            /* Convert AR to text */
            std::ostringstream ss;
            ss << cameraComponent.aspectRatio;
            std::string AR(ss.str());
            
            utils::DrawTableTextButton("Aspect Ratio", AR);
            utils::DrawTableFloat("Zoom Level", "Z", cameraComponent.zoomLevel);
            utils::DrawTableFloat("Moove Speed", "S", cameraComponent.mooveSpeed, true);
            
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    /* Transform */
    void ComponentInspector::DrawTransformComponent(void)
    {
        TransformComponent& transformComponent = m_Context->GetComponent<TransformComponent>();
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Transform Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            utils::DrawTableVec2("Position", transformComponent.position);
            utils::DrawTableVec2("Scale", transformComponent.size);
            float angleDeg = glm::degrees(transformComponent.rotationRadians);
            utils::DrawTableFloat("Rotation", "R", angleDeg);
            transformComponent.rotationRadians = glm::radians(angleDeg);
            
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    /* Sprite */
    void ComponentInspector::DrawSpriteComponent(void)
    {
        SpriteComponent& spriteComponent = m_Context->GetComponent<SpriteComponent>();
        
        //TODO
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Sprite Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::TextDisabled("TODO");
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    /* Behavior */
    void ComponentInspector::DrawBehaviorComponent(void)
    {
        BehaviorComponent& behaviorcomponent = m_Context->GetComponent<BehaviorComponent>();
        
        //TODO
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Behavior Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::TextDisabled("TODO");
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    
    /* ************************************************************************* */
    /* Add Component Button */
    void ComponentInspector::DrawButtonAddComponent()
    {
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponentPopUp");
        }
        
        /**
         * Pop Up
         * -> Display only the components that context entity does not have.
         */
        if (ImGui::BeginPopup("AddComponentPopUp"))
        {
            /* Transform */
            if (!hasTransformComponent)
            {
                if (ImGui::MenuItem("Transform"))
                {
                    m_Context->AddComponent<TransformComponent>();
                    ImGui::CloseCurrentPopup();
                    
                    hasTransformComponent = true;
                    LOG_TRACE("Transform added to {}", m_Context->GetId());
                }
            }
            
            /* Sprite */
            if (!hasSpriteComponent)
            {
                if (ImGui::MenuItem("Sprite"))
                {
                    m_Context->AddComponent<SpriteComponent>();
                    ImGui::CloseCurrentPopup();
                    
                    hasSpriteComponent = true;
                    LOG_TRACE("Sprite added to {}", m_Context->GetId());
                }
            }
            
            /* Behavior */
            if (!hasBehaviorComponent)
            {
                if (ImGui::MenuItem("Behavior"))
                {
                    m_Context->AddComponent<BehaviorComponent>();
                    ImGui::CloseCurrentPopup();
                    
                    hasBehaviorComponent = true;
                    LOG_TRACE("Behavior added to {}", m_Context->GetId());
                }
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
}
}