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

#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include <coconuts/graphics/BufferLayout.h>

#include <stdint.h>     /* uintptr_t */
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

namespace Coconuts
{
    
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _type)
    {
        switch(_type)
        {
            case ShaderDataType::None:      return 0;
            
            case ShaderDataType::Float:     return GL_FLOAT;
                
            case ShaderDataType::Float2:    return GL_FLOAT;
                
            case ShaderDataType::Float3:    return GL_FLOAT;
                
            case ShaderDataType::Float4:    return GL_FLOAT;
                
            case ShaderDataType::Mat3:      return GL_FLOAT;
                
            case ShaderDataType::Mat4:      return GL_FLOAT;
                
            case ShaderDataType::Int:       return GL_INT;
                
            case ShaderDataType::Int2:      return GL_INT;
                
            case ShaderDataType::Int3:      return GL_INT;
                
            case ShaderDataType::Int4:      return GL_INT;
                
            case ShaderDataType::Bool:      return GL_BOOL;
        }
        LOG_ERROR("Unknown ShaderDataType {}", _type);
        return 0;
    }
    
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
    }
    
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        
    }
        
    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }
    
    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
        
    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        
         uint32_t index = 0;
        for (const auto& element : vertexBuffer->GetLayout())
        {

            /* Enable a vertex attribute */
            glEnableVertexAttribArray(index);
            
            GLvoid* offset_ptr;
            offset_ptr = INT2VOIDP(element.offset); /* avoid compiler warnings due to casting */
            
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  vertexBuffer->GetLayout().GetStride(),
                                  offset_ptr);
            
            index++;
        }
         
         m_VertexBuffers.push_back(vertexBuffer);
         
         
    }
    
    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        
        m_IndexBuffers = indexBuffer;
    }
    
}