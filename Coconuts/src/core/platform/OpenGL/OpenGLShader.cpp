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

#include "OpenGLShader.h"
#include <glad/glad.h> 
#include <vector>
#include <coconuts/Logger.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace Coconuts
{
    
    OpenGLShader::OpenGLShader(const std::string& vertexShaderSrc,
                               const std::string& fragmentShaderSrc)
    {
        /**
         * Reference:
         *  https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
         */
        
        
        // Read our shaders into the appropriate buffers
        std::string vertexSource = vertexShaderSrc;     // Get source code for vertex shader.
        std::string fragmentSource = fragmentShaderSrc; // Get source code for fragment shader.

        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = (const GLchar *)vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(vertexShader);
            
            LOG_ERROR("{0}", infoLog.data());
            LOG_CRITICAL("Vertex Shader compilation failed!");
            exit(1);
            
            // Use the infoLog as you see fit.
            
            // In this simple program, we'll just leave
            //return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar *)fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            LOG_ERROR("{0}", infoLog.data());
            LOG_CRITICAL("Fragment Shader compilation failed!");
            exit(1);
            
            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            //return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        GLuint program = glCreateProgram();
        
        /* Save program ID */
        m_RendererID = program;

        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            LOG_ERROR("{0}", infoLog.data());
            LOG_CRITICAL("Shader link has failed!");
            exit(1);
            
            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            //return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }
    
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }
    
    void OpenGLShader::Bind()
    {
        glUseProgram(m_RendererID);
    }
    
    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }
    
    
    /**
     * Upload Uniforms
     */
        
    /**************/
    /*   Matrix   */
    /**************/
    // 2x2
    void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str()); 
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    // 3x3
    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str()); 
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    // 4x4
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str()); 
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    
    /**************/
    /*  Integer   */
    /**************/
    // 1
    void OpenGLShader::UploadUniformInt1(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value); 
    }
    
    
    /**************/
    /*   Float    */
    /**************/
    // 1
    void OpenGLShader::UploadUniformFloat1(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);   
    }
    
    // 2
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }
   
    // 3
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);   
    }
    
    // 4
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }
    
}