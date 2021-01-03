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
#ifndef COMPONENTINSPECTOR_H
#define COMPONENTINSPECTOR_H

#include <functional>

// ECS Components
#include <coconuts/ecs/components/TagComponent.h>
#include <coconuts/ecs/components/OrthoCameraComponent.h>
#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/SpriteComponent.h>
#include <coconuts/ecs/components/BehaviorComponent.h>

namespace Coconuts {
namespace Panels
{
    
    class ComponentInspector
    {
    public:
        ComponentInspector() = default;
        bool Init();
        
        void Draw();
        
        template <typename C>
        void ChangeContext(C* component);
        
    private:
        // Component Specific Drawing functions
        void DrawEmpty(void);
        void DrawTagComponent(void);
        void DrawCameraComponent(void);
        void DrawTransformComponent(void);
        void DrawSpriteComponent(void);
        void DrawBehaviorComponent(void);
        
        // Context
        std::function<void(void)> DrawComponentFunc;
        
        // Component references
        TagComponent*           tagComponent;
        OrthoCameraComponent*   cameraComponent;
        TransformComponent*     transformComponent;
        SpriteComponent*        spriteComponent;
        BehaviorComponent*      behaviorComponent;
    };
    
}
}

#endif /* COMPONENTINSPECTOR_H */

