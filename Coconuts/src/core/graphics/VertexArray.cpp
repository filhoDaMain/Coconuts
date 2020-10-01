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

#include <coconuts/graphics/VertexArray.h>
#include <coconuts/Renderer.h>
#include <coconuts/graphics/RendererAPI.h>
#include <coconuts/Logger.h>

// Platform - OpenGL
#include "OpenGLVertexArray.h"

namespace Coconuts
{
    
    //static
    VertexArray* VertexArray::Create()
    {
        switch(Renderer::GetRendererAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                return new OpenGLVertexArray();
                
                break;
            }
            
            default:
            {
                LOG_CRITICAL("VertexArray - Unknown RendererAPI {}", Renderer::GetRendererAPI());
                exit(1);
            }
        }
        
        return nullptr;
    }
    
}