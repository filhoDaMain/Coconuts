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
#ifndef LOWLEVELAPI_H
#define LOWLEVELAPI_H

#include <coconuts/graphics/RendererAPI.h>
#include <coconuts/graphics/VertexArray.h>

namespace Coconuts
{
    
    namespace Graphics
    {
        
        class LowLevelAPI
        {
        public:
            inline static void Init()
            {
                s_RendererAPI->Init();
            }
            
            inline static void SetClearColor(const glm::vec4& color)
            {
                s_RendererAPI->SetClearColor(color);
            }
        
            inline static void Clear()
            {
                s_RendererAPI->Clear();
            }
        
            inline static void DrawIndexed(const std::shared_ptr<VertexArray>&vertexArray)
            {
                s_RendererAPI->DrawIndexed(vertexArray);
            }
        
        private:
            static RendererAPI* s_RendererAPI;
        };
        
    }
        
}

#endif /* LOWLEVELAPI_H */

