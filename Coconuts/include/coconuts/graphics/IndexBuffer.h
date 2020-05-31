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
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <stdint.h>

namespace Coconuts
{
    
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual uint32_t GetCount() const = 0;
        
        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
    
}

#endif /* INDEXBUFFER_H */

