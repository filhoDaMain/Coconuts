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

#include "OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Coconuts
{
    
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count /* nr of uint32-t's */)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER    /* Index Buffer */,
                     count * sizeof(uint32_t)   /* Size (Bytes) of data */,
                     indices                    /* Data to be written in the Vertex Buffer */,
                     GL_STATIC_DRAW             /* Setup once */
                    );
    }
    
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

   
    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    
    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
}
