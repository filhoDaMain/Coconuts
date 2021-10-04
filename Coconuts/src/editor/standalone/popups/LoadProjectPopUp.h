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
#ifndef LOADPROJECTPOPUP_H
#define LOADPROJECTPOPUP_H

#include "../GameLayer.h"
#include "../panels/ComponentInspector.h"

namespace Coconuts {
namespace PopUps
{
    
    class LoadProjectPopUp
    {
    public:
        LoadProjectPopUp() = default;
        bool Init(GameLayer*& gameLayer, Panels::ComponentInspector* cmpInsp, bool* m_ShowPopUpLoadProj);
        void Draw();
        
    private:
        GameLayer* m_GameLayerPtr;
        Panels::ComponentInspector* m_ComponentInspectorPtr;
        bool* m_ShowPopUp;
    };
    
}
}

#endif /* LOADPROJECTPOPUP_H */

