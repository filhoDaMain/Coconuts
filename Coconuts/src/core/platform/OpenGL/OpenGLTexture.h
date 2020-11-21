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
#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include <coconuts/graphics/Texture.h>
#include <glad/glad.h>
#include <string>

namespace Coconuts
{
    
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();
        
        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        
        virtual void SetData(void* data, uint32_t size) override;
        
        void Bind(uint32_t slot = 0) const override;
        
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
    
}


#endif /* OPENGLTEXTURE_H */

