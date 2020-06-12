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
    
    class Shader
    {
    public:        
        static Shader* Create(const std::string& vertexShaderSrc,
                              const std::string& fragmentShaderSrc);
        
        virtual ~Shader() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        /**
         * Upload Uniforms
         */
        
        /**************/
        /*   Matrix   */
        /**************/
        // 2x2
        virtual void UploadUniformMat2(const std::string& name, const glm::mat2& matrix) = 0;
        // 3x3
        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
        // 4x4
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
        
        
        /**************/
        /*  Integer   */
        /**************/
        // 1
        virtual void UploadUniformInt1(const std::string& name, int value) = 0;
        
        
        /**************/
        /*   Float    */
        /**************/
        // 1
        virtual void UploadUniformFloat1(const std::string& name, float value) = 0;
        // 2
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values) = 0;
        // 3
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec3& values) = 0;
        // 4
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;
        
    private:
    };
    
}

#endif /* SHADER_H */

