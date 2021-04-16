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
#ifndef ASSETINSPECTOR_H
#define ASSETINSPECTOR_H

#include <string>
#include <functional>
#include "AssetInspector.h"

namespace Coconuts {
namespace Panels
{
 
    class AssetInspector
    {
    public:
        AssetInspector() = default;
        bool Init();
        
        void Draw();
        
        void ChangeContext2Texture2D(std::string& name);
        void ChangeContext2Sprite(std::string& name);
        
    private:
        std::function<void(void)> DrawContextFunc;
        
        void DrawTexture2DAsset();
        void DrawSpriteAsset();
        void DrawEmpty();
        
        /* Logical Names */
        std::string m_LogicalNameTexture2D;
        std::string m_LogicalNameSprite;
        
        bool isTexture2DSaved;
        bool isSpriteSaved;
        
    };
    
}
}

#endif /* ASSETINSPECTOR_H */

