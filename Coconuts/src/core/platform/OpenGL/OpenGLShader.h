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
#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include <coconuts/graphics/Shader.h>

namespace Coconuts
{
    enum class OpenGLShaderTypes
    {
        Unknown         = 0,
        GLSL_Vertex     = 1,
        GLSL_Fragment   = 2
    };
    
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader();
        
        OpenGLShader(const std::string& vertexShaderSrc,
                     const std::string& fragmentShaderSrc);
        
        virtual ~OpenGLShader();
        
        void AttachFromFile(ShaderTypes shaderType, const std::string& filepath) override;
        void DoneAttach() override;
        
        void Bind() override;
        void Unbind() override;
        
        /**
         * Upload Uniforms
         */
    
        /**************/
        /*   Matrix   */
        /**************/
        // 2x2
        void UploadUniformMat2(const std::string& name, const glm::mat2& matrix) override;
        // 3x3
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
        // 4x4
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
        
        
        /**************/
        /*  Integer   */
        /**************/
        // 1
        void UploadUniformInt1(const std::string& name, int value) override;
        
        
        /**************/
        /*   Float    */
        /**************/
        // 1
        void UploadUniformFloat1(const std::string& name, float value) override;
        // 2
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values) override;
        // 3
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
        // 4
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values) override;
        
    private:        
        void AttachVertexShader(const std::string& filepath) override;
        void AttachFragmentShader(const std::string& filepath) override;
        OpenGLShaderTypes ParseFileExtension(const std::string& filepath);
        
    private:
        uint32_t m_ProgramID;   // program ID
        uint32_t m_VertexID;    // vertex shader ID
        uint32_t m_FragmentID;  // fragment shader ID
    };
    
}

#endif /* OPENGLSHADER_H */

