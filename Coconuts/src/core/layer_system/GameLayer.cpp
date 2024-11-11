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
#include <coconuts/layer_system/GameLayer.h>
#include <coconuts/ECS.h>
#include <coconuts/Logger.h>
#include <coconuts/SceneManager.h>
#include <coconuts/ecs/Serializer.h>

// Debug Behavior
#include <coconuts/Polling.h>
#include <coconuts/Keyboard.h>


namespace Coconuts
{

    void GameLayer::OnUpdate(Timestep ts)
    {
        SceneManager::GetInstance().GetActiveScene()->OnUpdate(ts);
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
    
    float GameLayer::GetActiveSceneAR()
    {
        return SceneManager::GetInstance().GetActiveScene()->GetAspectRatio();
    }
    
}