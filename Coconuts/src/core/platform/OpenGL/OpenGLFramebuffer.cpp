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

#include "OpenGLFramebuffer.h"
#include <coconuts/Logger.h>
#include <glad/glad.h>

namespace Coconuts
{

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
        : m_Spec(spec), m_RendererID(0)
    {
        Invalidate();
    }
    
    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachID);
	glDeleteTextures(1, &m_DepthAttachID);
    }
    
    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Spec.width, m_Spec.height);
    }
    
    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void OpenGLFramebuffer::Resize(float width, float height)
    {
        m_Spec.width = width;
        m_Spec.width = height;
        Invalidate();
    }
    
    void OpenGLFramebuffer::Invalidate()
    {
        /* Framebuffer was previously initliazied */
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachID);
            glDeleteTextures(1, &m_DepthAttachID);
            // Create a new ...
        }
        
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
      
        /* Color buffer */
        glGenTextures(1, &m_ColorAttachID);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     (GLsizei)m_Spec.width,
                     (GLsizei)m_Spec.height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachID, 0);
        
        /* Depth buffer */
        glGenTextures(1, &m_DepthAttachID);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachID);
#if 0
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Spec.width, m_Spec.height);
#else
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH24_STENCIL8,
                     (GLsizei)m_Spec.width,
                     (GLsizei)m_Spec.height,
                     0,
                     GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8,
                     nullptr);
#endif
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachID, 0);

        if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        {
            LOG_CRITICAL("Framebuffer is incomplete!");
            /* Unbind Framebuffer */
            glBindFramebuffer(GL_TEXTURE_2D, 0);
            exit(1);
        }
        
        /* Unbind Framebuffer */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}