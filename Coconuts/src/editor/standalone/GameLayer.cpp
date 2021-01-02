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

#include <string>
#include <coconuts/Renderer.h>
#include "GameLayer.h"
#include <coconuts/ECS.h>
#include <coconuts/Logger.h>

#define GAMELAYER_SPRITESHEET_PATH  "../../Demo/examples/tiles2d/assets/textures/RPGpack_sheet_2X.png";

namespace Coconuts
{

    void GameLayer::OnUpdate(Timestep ts)
    {
        /* Bind Framebuffer */
        m_Framebuffer->Bind();

        m_ActiveScene->OnUpdate(ts);

        /* Unbind Framebuffer */
        m_Framebuffer->Unbind();
    }

    void GameLayer::OnEvent(Event& event)
    {
        m_ActiveScene->OnEvent(event);
    }
    
    GameLayer::GameLayer()
    {
    }

    void GameLayer::OnAttach()
    {   
        LOG_TRACE("Game Layer OnAttach()");
        
        /* Create Framebuffer */
        FramebufferSpecification spec;
        spec.width = 1280.0f;
        spec.height = 696.0f;
        m_Framebuffer.reset( Framebuffer::Create(spec) );
        
        /* Create a Scene */
        m_ActiveScene = std::make_shared<Scene>();
        
        /* Create an entity on Scene */
        m_Entity = Entity(m_ActiveScene, "Tree");
        
        /* Add TransformComponent */
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {0.5f, 1.0f};
        float rotationRadians = 0.79f;
        m_Entity.AddComponent<TransformComponent>(position, size, rotationRadians);
       
        /* Add SpriteComponent */
        /* 1) Init Spritesheet Texture */
        const std::string path = GAMELAYER_SPRITESHEET_PATH;
        std::shared_ptr<Texture2D> m_SpritesheetTexture2D;
        m_SpritesheetTexture2D.reset( Texture2D::Create(path) );
        
        /* 2) Create Sprite from the spritesheet */
        std::shared_ptr<Sprite> m_TreeSprite;
        m_TreeSprite.reset( Sprite::Create(m_SpritesheetTexture2D, {2, 1}, {128, 128}, {1, 2}) );
        
        /* 3) Assign the sprite to the Entity's SpriteComponent */
        m_Entity.AddComponent<SpriteComponent>(m_TreeSprite);
        
        
        /* Create a Camera Entity*/
        m_CameraEntity = Entity(m_ActiveScene);
        m_CameraEntity.AddComponent<OrthoCameraComponent>((float)(16.0f/9.0f), 1.0f);
        

        class TreeBehavior : public Behavior
        {
        public:
            void OnCreate()
            {
                
            }
            
            void OnDestroy()
            {
                
            }
            
            void OnUpdate(Timestep ts)
            {
                LOG_CRITICAL("Behavior OnUpdate() -> My ID:   {}", this->GetEntityId());
                LOG_CRITICAL("Behavior OnUpdate() -> My Tag:  {}", this->GetComponent<TagComponent>().tag);
            }
            
        private:
        };
        
        m_Entity.AddComponent<BehaviorComponent>();
        m_Entity.GetComponent<BehaviorComponent>().AddBehavior<TreeBehavior>(m_Entity);
    }

    void GameLayer::OnDetach()
    {

    }
    
    bool GameLayer::HaltEvents(bool state)
    {
        m_ActiveScene->HaltAllEvents(state);
        m_ActiveScene->HaltEditorCameraNavigation(state);
        return state;
    }
    
    void GameLayer::ChangeViewport(float x, float y)
    {
        m_ActiveScene->OnChangeViewport(x, y);
    }
    
}