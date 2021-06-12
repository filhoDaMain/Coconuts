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
#include <coconuts/AssetManager.h>
#include <coconuts/SceneManager.h>
#include <coconuts/ecs/Serializer.h>

// Debug Behavior
#include <coconuts/Polling.h>
#include <coconuts/Keyboard.h>


namespace Coconuts
{

    void GameLayer::OnUpdate(Timestep ts)
    {
        /* Bind Framebuffer */
        m_Framebuffer->Bind();

        SceneManager::GetInstance().GetActiveScene()->OnUpdate(ts);

        /* Unbind Framebuffer */
        m_Framebuffer->Unbind();
    }

    void GameLayer::OnEvent(Event& event)
    {
        SceneManager::GetInstance().GetActiveScene()->OnEvent(event);
    }
    
    GameLayer::GameLayer()
    {
    }

    void GameLayer::OnAttach()
    {   
        LOG_TRACE("Game Layer OnAttach()");
        
        /* Create Scene */
        SceneManager::GetInstance().NewScene("Example", true);
        
        /* Create Framebuffer */
        FramebufferSpecification spec;
        spec.width = 1280.0f;
        spec.height = 696.0f;
        m_Framebuffer.reset( Framebuffer::Create(spec) );

        
        /* Import Assets (example) */
        {
            std::string assetmanager_yaml = R"(
<AssetManager>:
  Textures2D List:
    - <Texture2D>:
        logicalName: Tiles_Spritesheet
        path: ../../Demo/assets/rpg/RPGpack_sheet_2X.png
        spritesUsing:
          - Tree
    - <Texture2D>:
        logicalName: Animals_Spritesheet
        path: ../../Demo/assets/animals/square_nodetailsOutline.png
        spritesUsing:
          - Pig
          - Chicken
  Sprites List:
    - <Sprite>:
        logicalName: Pig
        spriteSheetName: Animals_Spritesheet
        referrerIndex: 0x0
        spriteSelector:
          coords: [1, 4]
          cellSize: [136, 136]
          spriteSize: [1, 1]
    - <Sprite>:
        logicalName: Chicken
        spriteSheetName: Animals_Spritesheet
        referrerIndex: 0x1
        spriteSelector:
          coords: [4, 3]
          cellSize: [136, 136]
          spriteSize: [1, 1]
    - <Sprite>:
        logicalName: Tree
        spriteSheetName: Tiles_Spritesheet
        referrerIndex: 0x0
        spriteSelector:
          coords: [0, 0]
          cellSize: [140, 380]
          spriteSize: [1, 1])";
            
            AssetManager::Deserialize(assetmanager_yaml);
        }
        
            
        /* Import Scene (example) */
        {   
            std::string scene_yaml = R"(
<SceneManager>:
  Scenes List:
    - <Scene>:
        ID: 0x1
        Name: Untitled
        Entities List:
          - <Entity>:
              ID: 0x200000000
              TagComponent:
                tag: Sebastiao
              TransformComponent:
                position: [0, 0]
                size: [1, 1]
                rotationRadians: 0
              SpriteComponent:
                spriteLogicalName: Pig
                tintColor: [1, 1, 1, 1]
          - <Entity>:
              ID: 0x100000001
              TagComponent:
                tag: Camera
              OrthoCameraComponent:
                aspectRatio: 1.77777779
                zoomLevel: 1
                mooveSpeed: 1
                halt: false
                backgroundColor: [0, 0, 0, 1]
              TransformComponent:
                position: [0, 0]
                size: [1, 1]
                rotationRadians: 0)";
            
            std::shared_ptr<Scene> activeScene = SceneManager::GetInstance().GetActiveScene();
            Serializer serializer(activeScene);
            serializer.Deserialize(scene_yaml);
        }
        
    }

    void GameLayer::OnDetach()
    {

    }
    
    bool GameLayer::HaltEvents(bool state)
    {
        SceneManager::GetInstance().GetActiveScene()->HaltEditorCameraNavigation(state);
        SceneManager::GetInstance().GetActiveScene()->HaltAllEvents(state);
        return state;
    }
    
    void GameLayer::ChangeViewport(float x, float y)
    {
        SceneManager::GetInstance().GetActiveScene()->OnChangeViewport(x, y);
    }
    
    bool GameLayer::IsActiveSceneUpdated()
    {
        return SceneManager::GetInstance().GetActiveScene()->IsUpdated();
    }
    
    std::vector<Entity> GameLayer::GetActiveSceneEntities() const
    {
        return SceneManager::GetInstance().GetActiveScene()->GetAllEntities();
    }
    
    bool GameLayer::NewEntity()
    {
        /* Create on active scene */
        Entity empty(SceneManager::GetInstance().GetActiveScene().get());
        return true;
    }
    
    bool GameLayer::DestroyEntity(uint64_t id)
    {
        return SceneManager::GetInstance().GetActiveScene()->DestroyEntity(id);
    }
    
}