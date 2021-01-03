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
#ifndef SCENEOVERVIEW_H
#define SCENEOVERVIEW_H

#include "../GameLayer.h"
#include "ComponentInspector.h"
#include <coconuts/ecs/Entity.h>
#include <vector>

namespace Coconuts {
namespace Panels
{
    class SceneOverview
    {
    public:
        SceneOverview() = default;
        bool Init(GameLayer*& gameLayer, ComponentInspector* componentInspector);
        
        void Draw();
        
    private:
        void GetLastSceneUpdate();
        void DrawNode(Entity& entity);
        
        /* Pointer to the GameLayer */
        GameLayer* m_GameLayerPtr;
        
        /* Pointer to the Component Inspector Panel to change context */
        ComponentInspector* m_ComponentInspectorPtr;
        
        std::vector<Entity> sceneEntities;
    };
}
}

#endif /* SCENEOVERVIEW_H */

