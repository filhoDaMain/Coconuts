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
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <cstdint>
#include <coconuts/graphics/BufferLayout.h>

namespace Coconuts
{
    
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetData(const void* data, uint32_t size) = 0;
        
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        
        static VertexBuffer* Create(uint32_t size);
        static VertexBuffer* Create(float* vertices, uint32_t size);
    };
    
}

#endif /* VERTEXBUFFER_H */

