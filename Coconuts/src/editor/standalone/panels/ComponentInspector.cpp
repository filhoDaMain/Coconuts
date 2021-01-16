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
                DrawCameraComponent();
            }
            
            /* Transform */
            if (m_Context->HasComponent<TransformComponent>())
            {
                DrawTransformComponent();
            }
            
            /* Sprite */
            if (m_Context->HasComponent<SpriteComponent>())
            {
                DrawSpriteComponent();
            }
            
            /* Behavior */
            if (m_Context->HasComponent<BehaviorComponent>())
            {
                DrawBehaviorComponent();
            }
        }
        
        else
        {
            DrawEmpty();
        }
        
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
            DrawTableTextButton("Camera Type", "Othographic");
            
            /* Convert AR to text */
            std::ostringstream ss;
            ss << cameraComponent.aspectRatio;
            std::string AR(ss.str());
            
            DrawTableTextButton("Aspect Ratio", AR);
            DrawTableFloat("Zoom Level", "Z", cameraComponent.zoomLevel);
            DrawTableFloat("Moove Speed", "S", cameraComponent.mooveSpeed, true);
            
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
            DrawTableVec2("Position", transformComponent.position);
            DrawTableVec2("Scale", transformComponent.size);
            float angleDeg = glm::degrees(transformComponent.rotationRadians);
            DrawTableFloat("Rotation", "R", angleDeg);
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
    /* Helper functions */
    
    //static
    void ComponentInspector::DrawTableVec2(const std::string& label, glm::vec2& values, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button("X"))
        {
            values.x = 0.0f;
        }    
        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);

        if (ImGui::Button("Y"))
        {
            values.y = 0.0f;
        }    
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing(); 
    }
    
    //static
    void ComponentInspector::DrawTableFloat(const std::string& label, const std::string& param, float& values, bool positive, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button(param.c_str()))
        {
            values = 0.0f;
        }    
        ImGui::SameLine();
        if (positive)
        {
            ImGui::DragFloat("##X", &values, 0.1f, 0, 100);
        }
        else
        {
            ImGui::DragFloat("##X", &values, 0.1f);
        }
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
    //static
    void ComponentInspector::DrawTableTextButton(const std::string& label, const std::string& text, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button(text.c_str()))
        {
            
        }
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
}
}