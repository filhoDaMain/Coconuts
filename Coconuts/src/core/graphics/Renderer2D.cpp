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
        /* s_Data allocation and initialization */
        s_Data = new Renderer2DStorage();
        s_Data->batchRenderState.quadVertexBuffer_Base = new QuadVertex[s_Data->batchRenderState.maxVertices];
        s_Data->vertexArray.reset( VertexArray::Create() );
        s_Data->vertexBuffer.reset( VertexBuffer::Create(s_Data->batchRenderState.maxVertices * sizeof(QuadVertex)) );
        s_Data->shader.reset( Shader::Create() );
        
        /* Create a white texture of 1x1 pixel */
        uint32_t whiteTexData = 0xffffffff;
        s_Data->texture2D_Blank.reset(Texture2D::Create(1, 1, &whiteTexData, sizeof(whiteTexData)));
        /* Store it on Texture Slot index '0' */
        s_Data->batchRenderState.textureSlots[0] = s_Data->texture2D_Blank;
        
        /* Vertex Buffer layout -> Vertex Shader */
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },   /* QuadVertex:: glm::vec3 position */
            { ShaderDataType::Float4, "a_Color" },      /* QuadVertex:: glm::vec4 color */
            { ShaderDataType::Float2, "a_TexCoord" },   /* QuadVertex:: glm::vec2 texCoord */
            { ShaderDataType::Float, "a_TexIndex" },    /* QuadVertex:: foat texIndex */
            { ShaderDataType::Float, "a_TilingFactor" } /* QuadVertex:: foat tilingFactor */
        };
        
        /* Init VertexBuffer with the layout */
        s_Data->vertexBuffer->SetLayout(layout);

        /* Init Index Buffer with indices */
        std::shared_ptr<Coconuts::IndexBuffer> indexBuffer;
        uint32_t* indices = new uint32_t[s_Data->batchRenderState.maxIndices];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data->batchRenderState.maxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            
            offset += 4;
        }
        
        indexBuffer.reset( IndexBuffer::Create(indices, s_Data->batchRenderState.maxIndices) );
        delete[] indices;

        /* Set Vertex Array */
        s_Data->vertexArray->AddVertexBuffer(s_Data->vertexBuffer);
        s_Data->vertexArray->SetIndexBuffer(indexBuffer);

        /* Init quadVertexPositions */
        s_Data->batchRenderState.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
        
        /* Shaders setup */
        s_Data->shader->UseDefaultShaders();
        s_Data->shader->Bind();
          
        /* Array of texture units to be uploaded to the frag-shader as 2D-samplers */
        int samplers[s_Data->batchRenderState.maxTextureSlots];
        for (uint32_t i = 0; i < s_Data->batchRenderState.maxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        
        s_Data->shader->SetSamplers2D("u_Textures", samplers, s_Data->batchRenderState.maxTextureSlots);
        s_Data->shader->Unbind();
        
        
        
#if 0
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
#endif
    }
    
    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }
    
    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->shader->Bind();
        s_Data->shader->SetMat4("u_ViewProj", camera.GetViewProjMatrix());
        // Keep shader bound while Scene lasts!

        s_Data->batchRenderState.indicesCounter = 0;
        s_Data->batchRenderState.textureSlotsIndex = s_Data->batchRenderState.minTextureSlotIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr = s_Data->batchRenderState.quadVertexBuffer_Base;
    }
    
    void Renderer2D::EndScene()
    {
        uint32_t dataSize = (uint8_t*) s_Data->batchRenderState.quadVertexBuffer_Ptr - (uint8_t*) s_Data->batchRenderState.quadVertexBuffer_Base;
        
        /* Update Vertex Buffer */
        s_Data->vertexBuffer->SetData(s_Data->batchRenderState.quadVertexBuffer_Base, dataSize);
        
        /* Send to GPU */        
        Flush();
        
        s_Data->shader->Unbind();
    }
    
    void Renderer2D::Flush()
    {
        /* Bind all textures currently in use */
        for (uint32_t i = 0; i < s_Data->batchRenderState.textureSlotsIndex; i++)
        {
            /* Bind in the same shader texture-unit as its slot */
            s_Data->batchRenderState.textureSlots[i]->Bind(i);
        }
        
        /* Draw Call -> GPU */
        Graphics::LowLevelAPI::DrawIndexed(s_Data->vertexArray, s_Data->batchRenderState.indicesCounter);
        
        /* Update stats */
        s_Data->stats.drawCalls++;
    }
    
    void Renderer2D::FlushAndReset()
    {
        // Same as EndScene() without unbinding the shader
        {
            uint32_t dataSize = (uint8_t*) s_Data->batchRenderState.quadVertexBuffer_Ptr - (uint8_t*) s_Data->batchRenderState.quadVertexBuffer_Base;
        
            /* Update Vertex Buffer */
            s_Data->vertexBuffer->SetData(s_Data->batchRenderState.quadVertexBuffer_Base, dataSize);
        
            /* Send to GPU */        
            Flush();
        }
        
        // Same as BeginScene() withou re-binding the shader
        {
            s_Data->batchRenderState.indicesCounter = 0;
            s_Data->batchRenderState.textureSlotsIndex = s_Data->batchRenderState.minTextureSlotIndex;
            s_Data->batchRenderState.quadVertexBuffer_Ptr = s_Data->batchRenderState.quadVertexBuffer_Base;
        }
    }
    
    void Renderer2D::ResetStatistics()
    {
        memset(&(s_Data->stats), 0x00, sizeof(Statistics));
    }
    
    Statistics Renderer2D::GetStatistics()
    {
        return s_Data->stats;
    }
    
    // Color
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }
    
    // Color
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {   
        /* Flush (Draw Call) current vertex buffer state and start a new batch */
        if (s_Data->batchRenderState.indicesCounter >= s_Data->batchRenderState.maxIndices)
        {
            FlushAndReset();
        }
        
        /*
         * Each Quad consumes 4 vertexes.
         * Set them all.
         */
        
        float textureIndex = 0.0f;  // using blank texture slot
        float tilingFactor = 1.0f;
        
        /* Transform matrix */
        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        // 0
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
    // Color + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation_radians, const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation_radians, color);
    }
    
    // Color + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation_radians, const glm::vec4& color)
    {
        /* Flush (Draw Call) current vertex buffer state and start a new batch */
        if (s_Data->batchRenderState.indicesCounter >= s_Data->batchRenderState.maxIndices)
        {
            FlushAndReset();
        }
                
        /*
         * Each Quad consumes 4 vertexes.
         * Set them all.
         */
        
        float textureIndex = 0.0f;  // using blank texture slot
        float tilingFactor = 1.0f;
        
        /* Transform matrix */
        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), -rotation_radians, {0.0f, 0.0f, 0.1f})
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        // 0
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = color;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
    // Texture2D
    void Renderer2D::DrawQuad(const glm::vec2& position,
                              const glm::vec2& size,
                              const std::shared_ptr<Texture2D>& texture,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }
    
    // Texture2D
    void Renderer2D::DrawQuad(const glm::vec3& position,
                              const glm::vec2& size,
                              const std::shared_ptr<Texture2D>& texture,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {       
        /* Flush (Draw Call) current vertex buffer state and start a new batch */
        if (s_Data->batchRenderState.indicesCounter >= s_Data->batchRenderState.maxIndices)
        {
            FlushAndReset();
        }
        
        /*
         * Each Quad consumes 4 vertexes.
         * Set them all.
         */
        
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->batchRenderState.textureSlotsIndex; i++)
        {
            if (*s_Data->batchRenderState.textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float) i;
                
                // we found inside a texture slot, a texture equal (same ID)
                // to the texture we want to draw now
                break;
            }
        }
        
        /* If the texture is "new" (not set to a texture slot before) */
        if (textureIndex == 0.0f)
        {
            textureIndex = (float) s_Data->batchRenderState.textureSlotsIndex;
            
            /* Set it into a slot */
            s_Data->batchRenderState.textureSlots[s_Data->batchRenderState.textureSlotsIndex] = texture;
            s_Data->batchRenderState.textureSlotsIndex++;
        }
        
        /* Transform matrix */
        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        // 0
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
    // Texture2D + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor,
                             const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation_radians, texture, tilingFactor, tintColor);
    }
    
    // Texture2D + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor,
                             const glm::vec4& tintColor)
    {
        /* Flush (Draw Call) current vertex buffer state and start a new batch */
        if (s_Data->batchRenderState.indicesCounter >= s_Data->batchRenderState.maxIndices)
        {
            FlushAndReset();
        }
        
        /*
         * Each Quad consumes 4 vertexes.
         * Set them all.
         */
        
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->batchRenderState.textureSlotsIndex; i++)
        {
            if (*s_Data->batchRenderState.textureSlots[i].get() == *texture.get())
            {
                textureIndex = (float) i;
                
                // we found inside a texture slot, a texture equal (same ID)
                // to the texture we want to draw now
                break;
            }
        }
        
        /* If the texture is "new" (not set to a texture slot before) */
        if (textureIndex == 0.0f)
        {
            textureIndex = (float) s_Data->batchRenderState.textureSlotsIndex;
            
            /* Set it into a slot */
            s_Data->batchRenderState.textureSlots[s_Data->batchRenderState.textureSlotsIndex] = texture;
            s_Data->batchRenderState.textureSlotsIndex++;
        }
        
        /* Transform matrix */
        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), -rotation_radians, {0.0f, 0.0f, 0.1f})
                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        // 0
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 0.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {1.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = {0.0f, 1.0f};
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
}