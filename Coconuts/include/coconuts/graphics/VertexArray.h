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
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <memory>
#include <coconuts/graphics/VertexBuffer.h>
#include <coconuts/graphics/IndexBuffer.h>

namespace Coconuts
{
    
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
        
        static VertexArray* Create();
    };
    
}

#endif /* VERTEXARRAY_H */

