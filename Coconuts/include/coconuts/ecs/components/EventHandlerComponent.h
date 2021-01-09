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
#ifndef EVENTHANDLERCOMPONENT_H
#define EVENTHANDLERCOMPONENT_H

#include <coconuts/ecs/EventHandler.h>
#include <coconuts/EventSystem.h>
#include <functional>


namespace Coconuts
{
    
    struct EventHandlerComponent
    {
        //TODO make an array of instance pointers so an Entity can have multiple Event Handlers
        EventHandler* instance = nullptr;
        std::function<void()> Instantiate;
        std::function<void()> Destroy;
        
        std::function<void(EventHandler*)> OnCreateFunc;
        std::function<void(EventHandler*)> OnDestroyFunc;
        std::function<void(EventHandler*, Event&)> OnEventFunc;
        
        template <typename C>
        void AddHandler(Entity& affects)
        {   
            Instantiate = [&]() { instance = new C(); };
            Destroy = [&]() { delete (C*)instance; instance = nullptr; };
            
            OnCreateFunc = [](EventHandler* inst) { ((C*) inst)->OnCreate(); };
            OnDestroyFunc = [](EventHandler* inst) { ((C*) inst)->OnDestroy(); };
            OnEventFunc = [](EventHandler* inst, Event& e) { ((C*) inst)->OnEvent(e); };
            
            /* Instantiate object of EventHandler class */
            Instantiate();
            
            /* Link it to an entity affected by this handler */
            instance->SetAffectedEntity(affects);
        }
    };
    
}

#endif /* EVENTHANDLERCOMPONENT_H */

