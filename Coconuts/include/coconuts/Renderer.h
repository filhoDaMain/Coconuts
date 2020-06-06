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
#ifndef RENDERER_H
#define RENDERER_H

#include <coconuts/graphics/GraphicsContext.h>
#include <coconuts/graphics/RendererAPI.h>
#include <coconuts/graphics/LowLevelAPI.h>
#include <coconuts/graphics/VertexArray.h>
#include <memory>

namespace Coconuts
{
    
    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();
        static void Submit(const std::shared_ptr<VertexArray>&vertexArray);
        
        inline static RendererAPI::API GetRendererAPI() {return RendererAPI::GetAPI(); }
        
    private:
    };
    
}

#endif /* RENDERER_H */

