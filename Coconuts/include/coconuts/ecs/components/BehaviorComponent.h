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
#ifndef BEHAVIORCOMPONENT_H
#define BEHAVIORCOMPONENT_H

#include <coconuts/ecs/Behavior.h>
#include <coconuts/time/Timestep.h>
#include <coconuts/Logger.h>
#include <functional>


namespace Coconuts
{
    
    struct BehaviorComponent
    {
        //TODO make an array of instance pointers so an Entity can have multiple Behaviors
        Behavior* instance = nullptr;
        std::function<void()> Instantiate;
        std::function<void()> Destroy;
        
        std::function<void(Behavior*)> OnCreateFunc;
        std::function<void(Behavior*)> OnDestroyFunc;
        std::function<void(Behavior*, Timestep)> OnUpdateFunc;
        
        template <typename C>
        void AddBehavior(Entity& affects)
        {   
            Instantiate = [&]() { instance = new C(); };
            Destroy = [&]() { delete (C*)instance; instance = nullptr; };
            
            OnCreateFunc = [](Behavior* inst) { ((C*) inst)->OnCreate(); };
            OnDestroyFunc = [](Behavior* inst) { ((C*) inst)->OnDestroy(); };
            OnUpdateFunc = [](Behavior* inst, Timestep ts) { ((C*) inst)->OnUpdate(ts); };
            
            /* Instantiate object of Behavior class */
            Instantiate();
            
            /* Link it to an entity affected by this behavior */
            instance->SetAffectedEntity(affects);
        }
    };
    
}

#endif /* BEHAVIORCOMPONENT_H */

