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

#include <coconuts/ecs/Entity.h>
#include <coconuts/ecs/components/TagComponent.h>

namespace Coconuts
{
    
    Entity::Entity(Scene* scene, const std::string& name)
    : m_Scene(scene)
    {
        /* Create/Add *this* Entity to its Scene */
        m_EntityxEntity = m_Scene->CreateEntity();
        
        /* Add default Components */
        this->AddComponent<TagComponent>(name); // Entity tag
        
        m_Scene->SetUpdateFlag();
    }
    
    Entity::~Entity()
    {
        
    }
    
}