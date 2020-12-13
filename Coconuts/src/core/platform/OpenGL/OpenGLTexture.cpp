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
#include <stb/stb_image.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size)
        : m_Width(width), m_Height(height)
    {
        GLenum internalFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;
        
        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;
        
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        /* Specify a 2D Texture Image */        
        glTexImage2D(GL_TEXTURE_2D,
                     0, internalFormat,
                     m_Width, m_Height,
                     0,
                     dataFormat,
                     GL_UNSIGNED_BYTE,
                     data);
        
        /* Unbind Texture */
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
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
        
        GLenum internalFormat = 0;
        GLenum dataFormat = 0;
        
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        
        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;
        
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        /* Specify a 2D Texture Image */        
        glTexImage2D(GL_TEXTURE_2D,
                     0, internalFormat,
                     m_Width, m_Height,
                     0,
                     dataFormat,
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
    
    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        //glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        
        /* Specify a 2D Texture Image */        
        glTexImage2D(GL_TEXTURE_2D,
                     0, m_InternalFormat,
                     m_Width, m_Height,
                     0,
                     m_DataFormat,
                     GL_UNSIGNED_BYTE,
                     data);
    }
    
    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
}