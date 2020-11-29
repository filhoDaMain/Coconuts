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

#include <coconuts/graphics/Renderer2D.h>
#include <coconuts/graphics/LowLevelAPI.h>

#include "glm/ext/matrix_transform.hpp"

namespace Coconuts
{    
    
    static Renderer2DStorage* s_Data;
    
    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();
        
        s_Data->vertexArray_Quad.reset(VertexArray::Create());
        s_Data->shader_Texture.reset(Shader::Create());
          
        /* Create a white texture of 1x1 pixel */
        uint32_t whiteTexData = 0xffffffff;
        s_Data->texture2D_Blank.reset(Texture2D::Create(1, 1, &whiteTexData, sizeof(whiteTexData)));
        
         float vertices_quad[5 * 4] = {
        /* |---- a_Position ----|-- a_TexCoord --| */
            -0.5f, -0.5f,  0.0f,    0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f,    0.0f, 1.0f
        };
        
        /* a_Postion: vertex screen position (x,y,z) normalized in the [-1, 1] space */
        
        /* a_TexCoord: texture image edges (x,y) normalized in the [0, 1] space */
        
        /**
         * NOTE:
         * The idea is to map the edges of a square formed by two intersected triangles
         * (a_Position) to the edges of an image (a_TexCoord).
         * 
         * We then pick the texture's color of each pixel and paint the screen, by
         * interpolating the texture's pixels with corresponding square pixels on 
         * the screen.
         */
     
        
        /* -------------------------------------------------------------------- */
        /* Vertex Buffer (vb) */
        /* -------------------------------------------------------------------- */
        std::shared_ptr<Coconuts::VertexBuffer> vertexBuffer_Quad;
         
        vertexBuffer_Quad.reset(
                    VertexBuffer::Create(vertices_quad, sizeof(vertices_quad)));
        
        /* (Vertex) Buffer Layout -> Group vertices_square indices in a meaningful way */
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },   /* first 3 floats */
            { ShaderDataType::Float2, "a_TexCoord" }    /* next 2 floats */
        };
        
        /* Associate the layout with a buffer of vertices */
        vertexBuffer_Quad->SetLayout(layout);
       
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Index Buffer (ib) */
        /* -------------------------------------------------------------------- */
        uint32_t indices_quad[6] = {0, 1, 2, 2, 3, 0};
        
        /* Pick indices (lines) of vertices_square that can be re-used to form a square */
        
        /* Add indices to an ib */
        std::shared_ptr<Coconuts::IndexBuffer> indexBuffer_Quad;
        indexBuffer_Quad.reset(
                IndexBuffer::Create(indices_quad, sizeof(indices_quad)/sizeof(uint32_t)));
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Add vb and ib to VA */
        s_Data->vertexArray_Quad->AddVertexBuffer(vertexBuffer_Quad);
        s_Data->vertexArray_Quad->SetIndexBuffer(indexBuffer_Quad);
        /* -------------------------------------------------------------------- */
        
        /* Use default shaders */
        s_Data->shader_Texture->UseDefaultShaders();
        
        
        /* Create shader from files */
        //s_Data->shader_Texture->AttachFromFile(
        //    ShaderTypes::VERTEX, "../assets/shaders/Default.vert");
        
        //s_Data->shader_Texture->AttachFromFile(
        //    ShaderTypes::FRAGMENT, "../assets/shaders/Default.frag");
        
        //s_Data->shader_Texture->DoneAttach(); // FINISH Editing the Shader (Link them)
    }
    
    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }
    
    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {        
        s_Data->shader_Texture->Bind();
        s_Data->shader_Texture->SetMat4("u_ViewProj", camera.GetViewProjMatrix());
    }
    
    void Renderer2D::EndScene()
    {
        
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {   
        /* Set Color */
        s_Data->shader_Texture->SetFloat4("u_Color", color);
        
        /* Bind a white texture */
        s_Data->texture2D_Blank->Bind();
        
        glm::mat4 transform = glm::translate(
                glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f) /* * rotation */, 
                {size.x, size.y, 1.0f}
        );
        s_Data->shader_Texture->SetMat4("u_Transform", transform);
        
        s_Data->vertexArray_Quad->Bind();
        Graphics::LowLevelAPI::DrawIndexed(s_Data->vertexArray_Quad);
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture /*, glm::vec4 tintColor*/)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture /*, glm::vec4 tintColor*/)
    {
        /* Set taint color */
        s_Data->shader_Texture->SetFloat4("u_Color", glm::vec4(1.0f));
     
        /* Bind Texture */
        texture->Bind(0);
        
        glm::mat4 transform = glm::translate(
                glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f) /* * rotation */, 
                {size.x, size.y, 1.0f}
        );
        s_Data->shader_Texture->SetMat4("u_Transform", transform);
        
        s_Data->vertexArray_Quad->Bind();
        Graphics::LowLevelAPI::DrawIndexed(s_Data->vertexArray_Quad);
    }
    
}