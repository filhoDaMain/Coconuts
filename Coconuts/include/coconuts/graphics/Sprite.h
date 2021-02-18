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
#ifndef SPRITE_H
#define SPRITE_H

#include <coconuts/graphics/Texture.h>
#include <memory>
#include <glm/glm.hpp>

namespace Coconuts
{
    
    class Sprite
    {
    public:
        Sprite() = default;
        Sprite(const std::shared_ptr<Texture2D>& spriteSheet, 
               const glm::vec2& min,
               const glm::vec2& max);
        
        static Sprite* Create(const std::shared_ptr<Texture2D>& spriteSheet,
                              const glm::vec2& coords,
                              const glm::vec2& cellSize,
                              const glm::vec2& spriteSize = glm::vec2(1.0f));
        
        void UpdateData(const std::shared_ptr<Texture2D>& spriteSheet,
                        const glm::vec2& coords,
                        const glm::vec2& cellSize,
                        const glm::vec2& spriteSize);
        
        const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
        
    private:
        std::shared_ptr<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];
    };
    
}

#endif /* SPRITE_H */

