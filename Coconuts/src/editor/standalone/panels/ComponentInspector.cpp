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

namespace Coconuts {
namespace Panels
{
 
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
    
    void ComponentInspector::DrawCameraComponent(void)
    {
        //TODO
        ImGui::Text("Camera Component");
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawTransformComponent(void)
    {
        ImGui::Text("Transform Component");
        ImGui::Spacing();
        ImGui::Text("Position:  %f, %f", transformComponent->position.x, transformComponent->position.y);
        ImGui::Text("Rotation:  %f rads", transformComponent->rotationRadians);
        ImGui::Text("Size:      %f, %f", transformComponent->size.x, transformComponent->size.y);
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawSpriteComponent(void)
    {
        //TODO
        ImGui::Text("Sprite Component");
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    }
    
    void ComponentInspector::DrawBehaviorComponent(void)
    {
        //TODO
        ImGui::Text("Behavior Compoent");
        
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