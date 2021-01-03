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

#include "SceneOverview.h"

namespace Coconuts {
namespace Panels
{
 
    bool SceneOverview::Init(GameLayer*& gameLayer)
    {
        m_GameLayerPtr = gameLayer;
        
        return true;
    }
    
    void SceneOverview::LiveUpdate()
    {
        // Is Scene Updated?
        //      GetLastSceneUpdate();
        // Draw
    }
    
    void SceneOverview::GetLastSceneUpdate()
    {
        /* Get tmp vector from Game Layer with al entities */
        std::vector<Entity> last = m_GameLayerPtr->GetActiveSceneEntities();
        
        /* Free heap vector and re-allocate */
        sceneEntities = std::vector<Entity>();
        sceneEntities.resize(last.size());
        
        /* Copy */
        sceneEntities = last;
    }
    
}
}