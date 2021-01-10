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
    
    /* Helper functions */
    static void DrawTableVec2(const std::string& label, glm::vec2& values, float colWidth = 100.0f)
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
    
    static void DrawTableFloat(const std::string& label, const std::string& param, float& values, bool positive = false, float colWidth = 100.0f)
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
    
    static void DrawTableTextButton(const std::string& label, const std::string& text, float colWidth = 100.0f)
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
    
    
    bool ComponentInspector::Init()
    {
        DrawComponentFunc = [&]() { this->DrawEmpty(); };
        return true;
    }
    
    /* Draw */
    void ComponentInspector::Draw()
    {
        ImGui::Begin("Component Inspector");
        DrawComponentFunc();
        ImGui::End();
    }
    
    /* Draw All Components */
    void ComponentInspector::DrawAll()
    {
        ImGui::Begin("Component Inspector");
        
        if (hasTagComponent)
        {
            DrawTagComponent();
            hasTagComponent = false;
        }
        
        if (hasCameraComponent)
        {
            DrawCameraComponent();
            hasCameraComponent = false;
        }
        
        if (hasTransformComponent)
        {
            DrawTransformComponent();
            hasTransformComponent = false;
        }
        
        if (hasSpriteComponent)
        {
            DrawSpriteComponent();
            hasSpriteComponent = false;
        }
        
        if (hasBehaviorComponent)
        {
            DrawBehaviorComponent();
            hasBehaviorComponent = false;
        }
        
        ImGui::End();
    }
    
    /* Draw current context */
    void ComponentInspector::DrawEmpty(void)
    {
        //empty :)
    }
    
    /* Tag */
    void ComponentInspector::DrawTagComponent(void)
    {
        char buffer[16];
        memset(buffer, 0x00, sizeof(buffer));
        strcpy(buffer, tagComponent->tag.c_str());

        ImGui::Text("Tag");
        if (ImGui::InputText(" ", buffer, sizeof(buffer)))
        {
            tagComponent->tag = std::string(buffer);
        }

        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    /* Camera */
    void ComponentInspector::DrawCameraComponent(void)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Camera Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            DrawTableTextButton("Camera Type", "Othographic");
            
            /* Convert AR to text */
            std::ostringstream ss;
            ss << cameraComponent->aspectRatio;
            std::string AR(ss.str());
            
            DrawTableTextButton("Aspect Ratio", AR);
            DrawTableFloat("Zoom Level", "Z", cameraComponent->zoomLevel);
            DrawTableFloat("Moove Speed", "S", cameraComponent->mooveSpeed, true);
            
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawTransformComponent(void)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Transform Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            DrawTableVec2("Position", transformComponent->position);
            DrawTableVec2("Scale", transformComponent->size);
            float angleDeg = glm::degrees(transformComponent->rotationRadians);
            DrawTableFloat("Rotation", "R", angleDeg);
            transformComponent->rotationRadians = glm::radians(angleDeg);
            
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawSpriteComponent(void)
    {
        //TODO
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Sprite Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            //TODO
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawBehaviorComponent(void)
    {
        //TODO
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Behavior Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            //TODO
            
            ImGui::TreePop();
        }
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    
    /* Change Context */
    template<>
    void ComponentInspector::ChangeContext<TagComponent>(TagComponent* component)
    {
        tagComponent = component;
        DrawComponentFunc = [&]() { this->DrawTagComponent(); };
        hasTagComponent = true;
    }
    
    template<>
    void ComponentInspector::ChangeContext<OrthoCameraComponent>(OrthoCameraComponent* component)
    {
        cameraComponent = component;
        DrawComponentFunc = [&]() { this->DrawCameraComponent(); };
        hasCameraComponent = true;
    }
    
    template<>
    void ComponentInspector::ChangeContext<TransformComponent>(TransformComponent* component)
    {
        transformComponent = component;
        DrawComponentFunc = [&]() { this->DrawTransformComponent(); };
        hasTransformComponent = true;
    }
    
    template<>
    void ComponentInspector::ChangeContext<SpriteComponent>(SpriteComponent* component)
    {
        spriteComponent = component;
        DrawComponentFunc = [&]() { this->DrawSpriteComponent(); };
        hasSpriteComponent = true;
    }
    
    template<>
    void ComponentInspector::ChangeContext<BehaviorComponent>(BehaviorComponent* component)
    {
        behaviorComponent = component;
        DrawComponentFunc = [&]() { this->DrawBehaviorComponent(); };
        hasBehaviorComponent = true;
    }
    
}
}