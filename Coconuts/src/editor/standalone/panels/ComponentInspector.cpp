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
#include "glm/gtc/type_ptr.hpp"
#include <coconuts/editor.h>
#include <string.h>
#include <sstream>
#include <coconuts/AssetManager.h>
#include <coconuts/Logger.h>

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
                ImGui::Separator();
                ImGui::Spacing(); ImGui::Spacing();
                DrawTagComponent();
                ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing();
                ImGui::Separator();
            }
            
            /* Camera */
            if (m_Context->HasComponent<OrthoCameraComponent>())
            {
                hasCameraComponent = true;
                
                ImGui::Spacing(); ImGui::Spacing();
                DrawCameraComponent();
                ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing();
                ImGui::Separator();
            }
            
            /* Transform */
            if (m_Context->HasComponent<TransformComponent>())
            {
                hasTransformComponent = true;
                
                ImGui::Spacing(); ImGui::Spacing();
                DrawTransformComponent();
                ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing();
                ImGui::Separator();
            }
            
            /* Sprite */
            if (m_Context->HasComponent<SpriteComponent>())
            {
                hasSpriteComponent = true;
                
                ImGui::Spacing(); ImGui::Spacing();
                DrawSpriteComponent();
                ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing();
                ImGui::Separator();
            }
            
            /* Behavior */
            if (m_Context->HasComponent<BehaviorComponent>())
            {
                hasBehaviorComponent = true;
                
                ImGui::Spacing(); ImGui::Spacing();
                DrawBehaviorComponent();
                ImGui::Spacing(); ImGui::Spacing();ImGui::Spacing();
                ImGui::Separator();
            }
            
            ImGui::Spacing(); ImGui::Spacing();
            
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
    }
    
    /* Sprite */
    void ComponentInspector::DrawSpriteComponent(void)
    {
        SpriteComponent& spriteComponent = m_Context->GetComponent<SpriteComponent>();
        
        static bool saved = true;
        static bool undefined_sprite = false;
        
        //TODO
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx("Sprite Component" , flags);
 
        /* Draw Component */
        if (open)
        {
            ImGui::Spacing(); ImGui::Spacing();
            
            auto sprites = AssetManager::GetAllSpriteLogicalNames();
            std::vector<char*> spritesArray;
            spritesArray.reserve(sprites.size());
            
            static int seletected_sprite_index = 0;
            int i;
            for (i = 0; i < sprites.size(); i++)
            {
                spritesArray.push_back(const_cast<char*>(sprites[i].c_str()));
                
                if (saved && spriteComponent.spriteLogicalName.compare(spritesArray[i]) == 0)
                {
                    seletected_sprite_index = i;    // begin with selected sprite
                }
            }
            
            /**
             * Display "Undefined" if sprite pointer is not correctly
             * pointing to any valid sprite asset.
             */
            if (spriteComponent.sprite.expired())
            {
                
                spritesArray.push_back(const_cast<char*>("Undefined"));
                
                if (!undefined_sprite)
                {
                    seletected_sprite_index = spritesArray.size() - 1;
                    undefined_sprite = true;
                }
                
            }
            
            /* Display Dropdown with available / selected sprite asset */
            ImGui::Text("Asset");
            ImGui::Combo("Sprite", &seletected_sprite_index, &spritesArray[0], spritesArray.size());
            
            /* Save sprite asset switch */
            saved = false;
            if (ImGui::Button("Save"))
            {
                /* Change Sprite to selected */
                std::string name2string = spritesArray[seletected_sprite_index];
                
                /* Only switch sprite to a valid sprite name asset */
                if (name2string.compare("Undefined") != 0)
                {
                    spriteComponent.spriteLogicalName = name2string;
                    spriteComponent.sprite = AssetManager::GetSprite(name2string); 
                }
                
                saved = true;
            }
            
            ImGui::Spacing(); ImGui::Spacing(); 
            
            /* Tint Color */
            ImGui::Text("Tint Color");
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteComponent.tintColor));
            
            /* Tiling Factor */
            // Not suitable for sprites - re-think tilingFactor for Texture2D usecase only!
            //DrawTableFloat("Tiling Factor", "T", spriteComponent.tilingFactor);
            
            ImGui::TreePop();
        }
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