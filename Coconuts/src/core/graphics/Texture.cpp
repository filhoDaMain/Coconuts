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

#include <coconuts/graphics/Texture.h>
#include <coconuts/Renderer.h>
#include <coconuts/graphics/RendererAPI.h>
#include <coconuts/Layer.h>

// Platform - OpenGL
#include "OpenGLTexture.h"

namespace Coconuts
{
    
    Texture2D* Texture2D::Create(uint32_t width, uint32_t height, void* data, uint32_t size)
    {
        switch(Renderer::GetRendererAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return new OpenGLTexture2D(width, height, data, size);
                
                break;
            }
            
            default:
            {
                LOG_CRITICAL("Texture2D - Unknown RendererAPI {}", Renderer::GetRendererAPI());
                exit(1);
            }
        }
        
        return nullptr;
    }
    
    Texture2D* Texture2D::Create(const std::string& path)
    {
        switch(Renderer::GetRendererAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return new OpenGLTexture2D(path);
                
                break;
            }
            
            default:
            {
                LOG_CRITICAL("Texture2D - Unknown RendererAPI {}", Renderer::GetRendererAPI());
                exit(1);
            }
        }
        
        return nullptr;
    }
    
}