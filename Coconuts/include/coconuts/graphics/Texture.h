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
#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <memory>
#include <string>

namespace Coconuts
{
    
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        virtual void SetData(void* data, uint32_t size) = 0;
        
        virtual void Bind(uint32_t slot = 0) const = 0;
        
        virtual bool operator == (const Texture& other) const = 0;
        
        virtual explicit operator void*() const = 0;
    };
    
    class Texture2D : public Texture
    {
    public:
        static Texture2D* Create(uint32_t width, uint32_t height, void* data, uint32_t size);
        static Texture2D* Create(const std::string& path);
        
    protected:
        virtual bool IsValid() = 0;
    };
    
}

#endif /* TEXTURE_H */

