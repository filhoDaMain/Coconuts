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
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

namespace Coconuts
{
    
    struct FramebufferSpecification
    {
        uint32_t    width;
        uint32_t    height;
        uint32_t    samples = 1;
        bool        swapChainTarget = false;
    };
    
    class Framebuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static Framebuffer* Create(const FramebufferSpecification& spec);

        virtual const FramebufferSpecification& GetFramebufferSpecification() const = 0;
        virtual uint32_t GetColorAttachID() const = 0;
    };
    
}

#endif /* FRAMEBUFFER_H */

