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
#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <glm/glm.hpp>
#include <coconuts/graphics/VertexArray.h>
#include <memory>

namespace Coconuts
{
    
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1
        };
        
        virtual void Init() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
                
        inline static API GetAPI() { return s_API; };
        
    private:
        static API s_API;
        
    };
    
}

#endif /* RENDERERAPI_H */

