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

#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        :   m_Path(path)
    {
        int width, height, channels;
        
    /**
     * Flip Texture vertically because OpenGL considers
     * the bottom left pixel to be the pixel (0, 0)
     */
    stbi_set_flip_vertically_on_load(1);
    
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        
        if (data == nullptr)
        {
            LOG_ERROR("OpenGLTexture2D - Could not load image file! ( {} )", path);
            LOG_ERROR("{}", stbi_failure_reason());
            return;
        }
        
        // Success
        m_Width = width;
        m_Height = height;
        
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        /* Specify a 2D Texture Image */
        glTexImage2D(GL_TEXTURE_2D,
                     0, GL_RGBA8 /* Internal Format: How OpenGL will store the data */,
                     m_Width, m_Height,
                     0,
                     GL_RGBA     /* Format: How Texture data (m_LocalBuffer) is encoded */,
                     GL_UNSIGNED_BYTE,
                     data);
        
        /* Unbind Texture */
        glBindTexture(GL_TEXTURE_2D, 0);
        
        if (data)
        {
            stbi_image_free(data);
        }
    }
    
    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }
    
    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
}