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
#ifndef FILEMENU_H
#define FILEMENU_H

#include "../GameLayer.h"

namespace Coconuts {
namespace MenuBar
{
    
    class FileMenu
    {
    public:
        FileMenu() = default;
        bool Init(GameLayer*& gameLayer);
        
        void Draw();
        
    private:
        void Close();
        
    private:
        /* Pointer to the GameLayer */
        GameLayer* m_GameLayerPtr;
    };
    
}
}

#endif /* FILEMENU_H */

