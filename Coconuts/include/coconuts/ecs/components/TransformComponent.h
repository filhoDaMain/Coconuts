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
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

namespace Coconuts
{
    
    struct TransformComponent
    {
        //data
        glm::vec2   position;
        glm::vec2   size;
        float       rotationRadians;
        
        TransformComponent() = default;
        TransformComponent(const glm::vec2& pos, const glm::vec2& sz = glm::vec2(1.0f), float rotationInRadians = 0.0f)
        : position(pos), size(sz), rotationRadians(rotationInRadians) {}
    };
    
}

#endif /* TRANSFORMCOMPONENT_H */

