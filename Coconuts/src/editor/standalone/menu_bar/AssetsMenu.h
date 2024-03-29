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
#ifndef ASSETSMENU_H
#define ASSETSMENU_H

namespace Coconuts {
namespace MenuBar
{
    
    class AssetsMenu
    {
    public:
        AssetsMenu() = default;
        bool Init(bool* showPopUp_ImportTexture2D, bool* showPopUp_CreateSprite)
        {
            m_ShowPopUp_ImportTexture2D = showPopUp_ImportTexture2D;
            m_ShowPopUp_CreateSprite = showPopUp_CreateSprite;
            return true;
        }
        
        void Draw();
        
    private:
        bool* m_ShowPopUp_ImportTexture2D;
        bool* m_ShowPopUp_CreateSprite;
    };
    
}
}

#endif /* ASSETSMENU_H */

