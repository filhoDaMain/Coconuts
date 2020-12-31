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
#ifndef BEHAVIORAL_H
#define BEHAVIORAL_H

#include <coconuts/ecs/Entity.h>
#include <coconuts/ecs/components/BehaviorComponent.h>

namespace Coconuts
{
    
    class Behavioral
    {
    public:
        
        template <typename C, typename ... Args>
        void AddComponent(Args && ... args)
        {
            m_Entity.AddComponent<C>();
        }
        
        template <typename C>
        C& GetComponent()
        {
            return m_Entity.GetComponent<C>();
        }
        
        template <typename C>
        bool HasComponent()
        {
            return m_Entity.HasComponent<C>();
        }
        
        uint64_t GetAffectedEntityId() const
        {
            return m_Entity.GetId();
        }
        
    private:
        Entity m_Entity;    // Linked with an Entity / Entities (future)
        friend struct BehaviorComponent;
    };
    
}

#endif /* BEHAVIORAL_H */

