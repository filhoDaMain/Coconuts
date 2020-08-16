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

#include <coconuts/Renderer.h>
#include <coconuts/graphics/LowLevelAPI.h>
#include <coconuts/Logger.h>
#include "OpenGLShader.h"

namespace Coconuts
{
    
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    
    // static
    void Renderer::Init()
    {
        Graphics::LowLevelAPI::Init();
    }
    
    // static
    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->viewProjMatrix = camera.GetViewProjMatrix();
    }
    
    // static
    void Renderer::EndScene()
    {
        
    }
    
    // static
    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
                          const std::shared_ptr<VertexArray>&vertexArray,
                          const glm::mat4& transform)
    {
        shader->Bind();
        
        switch(Renderer::GetRendererAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProj", m_SceneData->viewProjMatrix);
                std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
                break;
            }
            
            default:
            {
                LOG_CRITICAL("Renderer - Unknown RendererAPI {}", Renderer::GetRendererAPI());
                exit(1);
            }
        }
        
        vertexArray->Bind();
        Graphics::LowLevelAPI::DrawIndexed(vertexArray);
    }
    
}