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
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <coconuts/graphics/Sprite.h>
#include <glm/glm.hpp>
#include <memory>

namespace Coconuts
{
    
    struct SpriteComponent
    {
        //data
        std::shared_ptr<Sprite> sprite;
        glm::vec4               tintColor;
        float                   tilingFactor;
        
        SpriteComponent() = default;
        SpriteComponent(const std::shared_ptr<Sprite>& spr,
                        const glm::vec4& color = glm::vec4(1.0f),
                        float factor = 1.0f)
        : sprite(spr), tintColor(color), tilingFactor(factor) {}
    };
    
}

#endif /* SPRITECOMPONENT_H */

