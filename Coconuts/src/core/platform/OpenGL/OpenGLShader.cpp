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
#include <string>
#include <fstream>
#include <streambuf>

namespace Coconuts
{
    OpenGLShader::OpenGLShader()
    {
        /**
         * Reference:
         *  https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
         */
        
        /* Create program object */
        m_ProgramID = glCreateProgram();
        
        if (m_ProgramID == 0)
        {
            LOG_ERROR("Failed to create a shader program!");
            return;
        }
        
        /* SUCCESS */
        LOG_DEBUG("New shader program created ( {} )", m_ProgramID);
        LOG_TRACE("* m_ProgramID:    {}", m_ProgramID);
    }

    
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
        m_ProgramID = program;

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
    
    void OpenGLShader::AttachFromFile(ShaderTypes shaderType, const std::string& filepath)
    {
        LOG_TRACE("Attaching new shader from file: {}", filepath);
        
        /* Parse file extension */
        switch(ParseFileExtension(filepath))
        {
            case OpenGLShaderTypes::GLSL_Vertex:
            {
                if (ShaderTypes::VERTEX == shaderType)
                {   
                    AttachVertexShader(filepath);
                }
                else
                {
                    LOG_ERROR("Shader file extension does not match provided shader type!");
                    return;
                }
            }
            break;
            
            case OpenGLShaderTypes::GLSL_Fragment:
            {
                if (ShaderTypes::FRAGMENT == shaderType)
                {                    
                    AttachFragmentShader(filepath);
                }
                else
                {
                    LOG_ERROR("Shader file extension does not match provided shader type!");
                    return;
                }
            }
            break;
            
            case OpenGLShaderTypes::Unknown:
            default:
            {
                LOG_ERROR("Unknown shader file extension!");
                return;
            }
        }
    }
    
    void OpenGLShader::AttachVertexShader(const std::string& filepath)
    {
        /**
         * Reference:
         *  https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
         */
        
        /* A Shader program must exist */
        if (m_ProgramID == 0)
        {
            LOG_ERROR("A Shader program must exist before attaching shaders to it!");
            return;
        }
        
        /* Read file into sourceCode string*/
        std::ifstream t(filepath);
        std::string sourceCode((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
        t.close();
        
        /* Create an empty vertex shader handle */
        m_VertexID = 0;   // reset
        m_VertexID = glCreateShader(GL_VERTEX_SHADER);
        
        if (0 == m_VertexID)
        {
            LOG_ERROR("Error creating a vertex shader object!");
            return;
        }
        
        /* Send the vertex shader source code to GL */
        const GLchar *source = (const GLchar *)sourceCode.c_str();
        glShaderSource(m_VertexID, 1, &source, 0);
        
        /* Compile the vertex shader */
        glCompileShader(m_VertexID);
        
        /* Check for compilation errors */
        GLint isCompiled = 0;
        glGetShaderiv(m_VertexID, GL_COMPILE_STATUS, &isCompiled);
        
        /* Compilation FAILED */
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(m_VertexID, GL_INFO_LOG_LENGTH, &maxLength);
            
            /* The maxLength includes the NULL character */
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(m_VertexID, maxLength, &maxLength, &infoLog[0]);
            
            /* We don't need the shader anymore. */
            glDeleteShader(m_VertexID);
            
            LOG_ERROR("Vertex Shader compilation failed!");
            LOG_ERROR("* m_ProgramID:    {}", m_ProgramID);
            LOG_ERROR("* m_VertexID:     {}", m_VertexID);
            LOG_ERROR("* FILE: {}", filepath);
            LOG_ERROR("* ELOG: {}", infoLog.data());
            
            return;
        }
        
        /* SUCCESS */
        LOG_DEBUG("Vertex shader successfully compiled ( {} )", m_VertexID);
        LOG_TRACE("* m_ProgramID:    {}", m_ProgramID);
        LOG_TRACE("* m_VertexID:     {}", m_VertexID);
        LOG_TRACE("* FILE: {}", filepath); 
    }
    
    void OpenGLShader::AttachFragmentShader(const std::string& filepath)
    {
        /**
         * Reference:
         *  https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
         */
        
        /* A Shader program must exist */
        if (m_ProgramID == 0)
        {
            LOG_ERROR("A Shader program must exist before attaching shaders to it!");
            return;
        }
        
        /* Read file into sourceCode string*/
        std::ifstream w(filepath);
        std::string sourceCode((std::istreambuf_iterator<char>(w)),
        std::istreambuf_iterator<char>());
        w.close();
        
        /* Create an empty fragment shader handle */
        m_FragmentID = 0;   // reset
        m_FragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        
        if (0 == m_FragmentID)
        {
            LOG_ERROR("Error creating a fragment shader object!");
            return;
        }
        
        /* Send the fragment shader source code to GL */
        const GLchar *source = (const GLchar *)sourceCode.c_str();
        glShaderSource(m_FragmentID, 1, &source, 0);
        
        /* Compile the fragment shader */
        glCompileShader(m_FragmentID);
        
        /* Check for compilation errors */
        GLint isCompiled = 0;
        glGetShaderiv(m_FragmentID, GL_COMPILE_STATUS, &isCompiled);
        
        /* Compilation FAILED */
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(m_FragmentID, GL_INFO_LOG_LENGTH, &maxLength);
            
            /* The maxLength includes the NULL character */
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(m_FragmentID, maxLength, &maxLength, &infoLog[0]);
            
            /* We don't need the shader anymore. */
            glDeleteShader(m_FragmentID);
            
            LOG_ERROR("Fragment Shader compilation failed!");
            LOG_ERROR("* m_ProgramID:    {}", m_ProgramID);
            LOG_ERROR("* m_FragmentID:   {}", m_FragmentID);
            LOG_ERROR("* FILE: {}", filepath);
            LOG_ERROR("* ELOG: {}", infoLog.data());
            
            return;
        }
        
        /* SUCCESS */
        LOG_DEBUG("Fragment shader successfully compiled ( {} )", m_FragmentID);
        LOG_TRACE("* m_ProgramID:    {}", m_ProgramID);
        LOG_TRACE("* m_FragmentID:   {}", m_FragmentID);
        LOG_TRACE("* FILE: {}", filepath); 
    }
    
    OpenGLShaderTypes OpenGLShader::ParseFileExtension(const std::string& filepath)
    {
        std::string fileExt = filepath.substr(filepath.find_last_of(".") + 1);
        
        if (fileExt == "vert")
        {
            return OpenGLShaderTypes::GLSL_Vertex;
        }
        
        else if ( (fileExt == "frag") || (fileExt == "pixel"))
        {
            return OpenGLShaderTypes::GLSL_Fragment;
        }
        
        return OpenGLShaderTypes::Unknown;
        
    }
    
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_ProgramID);
    }
    
    void OpenGLShader::DoneAttach()
    {   
        /* A Shader program must exist */
        if (m_ProgramID == 0)
        {
            LOG_ERROR("A Shader program must exist before attaching shaders to it!");
            return;
        }
        
        /* A Vertex shader must exist */
        if (m_VertexID == 0)
        {
            LOG_ERROR("A Vertex Shader must exist before link program!");
            return;
        }
        
        /* A Fragment shader must exist */
        if (m_FragmentID == 0)
        {
            LOG_ERROR("A Fragment Shader must exist before link program!");
            return;
        }
        
        /* Attach shaders to program */
        glAttachShader(m_ProgramID, m_VertexID);
        glAttachShader(m_ProgramID, m_FragmentID);

        /* Link program */
        glLinkProgram(m_ProgramID);

        /* Check for link error */
        GLint isLinked = 0;
        glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int *)&isLinked);
        
        /* Link FAILED */
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

            /* The maxLength includes the NULL character */
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);

            /* We don't need the program anymore */
            glDeleteProgram(m_ProgramID);
            glDeleteShader(m_VertexID);
            glDeleteShader(m_FragmentID);

            LOG_ERROR("Shader program Link failed!");
            LOG_ERROR("* m_ProgramID:    {}", m_ProgramID);
            LOG_ERROR("* m_VertexID:     {}", m_VertexID);
            LOG_ERROR("* m_FragmentID:   {}", m_FragmentID);
            LOG_ERROR("* ELOG: {}", infoLog.data());
            
            return;
        }
        
        /* SUCCESS */

        /* Always detach shaders after a successful link */
        glDetachShader(m_ProgramID, m_VertexID);
        glDetachShader(m_ProgramID, m_FragmentID);
        
        LOG_DEBUG("Shader program successfully linked and ready to Bind");
        LOG_TRACE("* m_ProgramID:    {}", m_ProgramID);
        LOG_TRACE("* m_VertexID:     {}", m_VertexID);
        LOG_TRACE("* m_FragmentID:   {}", m_FragmentID);
        LOG_TRACE("* Not bound yet!");
    }
    
    void OpenGLShader::Bind()
    {
        glUseProgram(m_ProgramID);
        return;
    }
    
    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }
    
    
    /**
     * Set Uniforms
     * - Interface Implementation
     */
    
    void OpenGLShader::SetFloat1(const std::string& name, float value)
    {
        UploadUniformFloat1(name, value);
    }
    
    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
    {
        UploadUniformFloat2(name, values);
    }
    
    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
    {
        UploadUniformFloat3(name, values);
    }
    
    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
    {
        UploadUniformFloat4(name, values);
    }
    
    void OpenGLShader::SetInt1(const std::string& name, int value)
    {
        UploadUniformInt1(name, value);
    }
    
    void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& matrix)
    {
        UploadUniformMat2(name, matrix);
    }
    
    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
    {
        UploadUniformMat3(name, matrix);
    }
    
    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
    {
        UploadUniformMat4(name, matrix);
    }
    
    
    
    /**
     * Upload Uniforms 
     * - Interface Implementation
     */
        
    void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str()); 
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str()); 
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str()); 
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void OpenGLShader::UploadUniformFloat1(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform1f(location, value);   
    }
    
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }
   
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);   
    }
    
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }
    
    void OpenGLShader::UploadUniformInt1(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
        glUniform1i(location, value); 
    }
    
}