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
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <stdint.h>

namespace Coconuts
{
    
    class Shader
    {
    public:
        Shader(const std::string& vertexShaderSrc,
               const std::string& fragmentShaderSrc);
        
        ~Shader();
        
        void Bind();
        void Unbind();
        
    private:
        uint32_t m_RendererID;
    };
    
}

#endif /* SHADER_H */

