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

#include <coconuts/graphics/Sprite.h>

namespace Coconuts
{
    
    Sprite::Sprite(const std::shared_ptr<Texture2D>& spriteSheet, 
                   const glm::vec2& min,
                   const glm::vec2& max)
        : m_Texture(spriteSheet)
    {
        m_TexCoords[0] = {min.x, min.y};
        m_TexCoords[1] = {max.x, min.y};
        m_TexCoords[2] = {max.x, max.y};
        m_TexCoords[3] = {min.x, max.y};
    }
    
    // static
    Sprite* Sprite::Create(const std::shared_ptr<Texture2D>& spriteSheet,
                           const glm::vec2& coords,
                           const glm::vec2& cellSize,
                           const glm::vec2& spriteSize)
    {
        glm::vec2 min = { (coords.x * cellSize.x) / spriteSheet->GetWidth(),
                          (coords.y * cellSize.y) / spriteSheet->GetHeight() };
        
        glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / spriteSheet->GetWidth(),
                          ((coords.y + spriteSize.y) * cellSize.y) / spriteSheet->GetHeight() };
        
        return new Sprite(spriteSheet, min, max);
    }
    
}