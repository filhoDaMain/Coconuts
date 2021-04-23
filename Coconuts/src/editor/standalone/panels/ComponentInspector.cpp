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
#include <coconuts/graphics/defs.h>
#include "../ed_utils.h"

namespace Coconuts {
namespace Panels
{
     
    bool ComponentInspector::Init()
    {
        isSpriteComponentSaved = true;
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
            utils::DrawTableTextButton("Camera Type", "Othographic");
            
            /* Convert AR to text */
            std::ostringstream ss;
            ss << cameraComponent.aspectRatio;
            std::string AR(ss.str());
            
            utils::DrawTableTextButton("Aspect Ratio", AR);
            utils::DrawTableFloat("Zoom Level", "Z", cameraComponent.zoomLevel);
            utils::DrawTableFloat("Moove Speed", "S", cameraComponent.mooveSpeed, true);
            
            ImGui::Spacing(); ImGui::Spacing();
            
            /* Background Clear Color */
            ImGui::Text("Background Color");
            ImGui::ColorEdit4("Color", glm::value_ptr(cameraComponent.backgroundColor));
            
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
            utils::DrawTableVec2("Position", transformComponent.position);
            utils::DrawTableVec2("Scale", transformComponent.size);
            float angleDeg = glm::degrees(transformComponent.rotationRadians);
            utils::DrawTableFloat("Rotation", "R", angleDeg);
            transformComponent.rotationRadians = glm::radians(angleDeg);
            
            ImGui::TreePop();
        }
    }
    
    /* Sprite */
    void ComponentInspector::DrawSpriteComponent(void)
    {
        SpriteComponent& spriteComponent = m_Context->GetComponent<SpriteComponent>();
        
        static bool undefined_sprite = false;
        static glm::vec4 tint;
        
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
                spritesArray.emplace_back(const_cast<char*>(sprites[i].c_str()));
                
                if (isSpriteComponentSaved && spriteComponent.spriteLogicalName.compare(spritesArray[i]) == 0)
                {
                    seletected_sprite_index = i;    // 1st option to display
                }
            }
            
            /**
             * Display "Undefined" if sprite pointer is not correctly
             * pointing to any valid sprite asset.
             */
            if (spriteComponent.sprite.expired())
            {
                
                spritesArray.emplace_back(const_cast<char*>("Undefined"));
                
                if (!undefined_sprite)
                {
                    seletected_sprite_index = spritesArray.size() - 1;  // 1st option to display
                    undefined_sprite = true;
                }
                
            }
            
            /* Display Dropdown with available / selected sprite asset */
            ImGui::Text("Sprite Asset");
            ImGui::Combo("Sprite", &seletected_sprite_index, &spritesArray[0], spritesArray.size());
            
            /* Cast selected sprite name to a string */
            std::string name2string = spritesArray[seletected_sprite_index];
            
            /* Get sprite's spritesheet texture and sprite selector to preview it */
            bool undefined = true;
            if (name2string.compare("Undefined") != 0)  // not hovering over "Undefined"
            {
               do
               {
                   std::string spriteSheetName;
                   bool valid;
                   std::tie(valid, spriteSheetName) = AssetManager::GetSpriteSheetName(name2string);
                   if (!valid) break;
                   auto texture = AssetManager::GetTexture2D(spriteSheetName);
                   
                   AssetManager::SpriteSelector selector;
                   std::tie(valid, selector) = AssetManager::GetSpriteSelector(name2string);
                   if (!valid) break;
                   
                   if (texture != nullptr)
                   {
                       ImGui::Spacing(); ImGui::Spacing();
                       
                       /* Preview selected sprite */
                       utils::DrawTableImage("\nSprite\nPreview", *texture, selector, tint);
                       undefined = false;
                   }
               } while(false);
            }
            
            if (undefined)
            {
                ImGui::Spacing(); ImGui::Spacing();
                
                /* Show default "Missing Sprite" as preview */
                utils::DrawTableImage("\nSprite\nPreview", defs::DefaultMissingSpriteTexture());
            }
            
            ImGui::Spacing(); ImGui::Spacing(); 
            
            /* Tint Color */
            /* First time, get original tint color from spritecomponent */
            if (isSpriteComponentSaved)
            {
                tint = spriteComponent.tintColor;
            }
            ImGui::Text("Tint Color");
            ImGui::ColorEdit4("Color", glm::value_ptr(tint));
            
            ImGui::Spacing(); ImGui::Spacing(); 
            
            /* Save sprite asset switch */
            isSpriteComponentSaved = false;
            if (ImGui::Button("Apply"))
            {   
                /* Update to newly selected sprite from drop-down list */
                
                /* Only switch sprite to a valid sprite name asset */
                if (name2string.compare("Undefined") != 0)
                {
                    spriteComponent.spriteLogicalName = name2string;
                    spriteComponent.sprite = AssetManager::GetSprite(name2string);
                    
                    /* Update selected tint color */
                    spriteComponent.tintColor = tint;
                    undefined_sprite = false;
                }
                
                isSpriteComponentSaved = true;
            }
            
            /* Tiling Factor */
            // Not suitable for sprites - re-think tilingFactor for Texture2D usecase only!
            
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
    
    void ComponentInspector::ChangeContext(Coconuts::Entity*& ptr)
    {
        LOG_TRACE("ComponentInspector - Change context to Entity {}", ptr->GetId());
        m_Context = ptr;
        hasValidContext = true;
        isSpriteComponentSaved = true;  // fetch original spritecomponent data
    }
    
}
}