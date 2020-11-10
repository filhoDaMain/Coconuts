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
#include <glm/glm.hpp>


namespace Coconuts
{
    enum class ShaderTypes
    {
        VERTEX      = 0,
        FRAGMENT    = 1
    };
    
    class Shader
    {
    public:
        static Shader* Create();
        
        static Shader* Create(const std::string& vertexShaderSrc,
                              const std::string& fragmentShaderSrc);
        
        virtual ~Shader() = default;
        
        virtual void AttachFromFile(ShaderTypes shaderType, const std::string& filepath) = 0;
        virtual void DoneAttach() = 0;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        
        /**
         * Set Uniforms
         */
        
        virtual void SetFloat1(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
        
        virtual void SetInt1(const std::string& name, int value) = 0;
        
        virtual void SetMat2(const std::string& name, const glm::mat2& matrix) = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
        
    private:
        virtual void AttachVertexShader(const std::string& filepath) = 0;
        virtual void AttachFragmentShader(const std::string& filepath) = 0;
    };
    
}

#endif /* SHADER_H */

