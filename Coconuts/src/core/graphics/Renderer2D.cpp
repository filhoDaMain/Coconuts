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
#include <string>

const std::string __PROJ_BASEDIR    = COCONUTS_BUILDTREE_ROOTDIR;
const std::string __ASSET_REL_PATH  = "/include/coconuts/graphics/Warning_MissingSprite.png";
const std::string MISSING_SPRITE_TEXTURE_PATH = __PROJ_BASEDIR + __ASSET_REL_PATH;

namespace Coconuts
{    
    
    /* Static definitions */
    std::shared_ptr<Texture2D> Renderer2D::s_WarningMissingSpriteTexture;
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
        
        /* Init default texture triggered by a missing sprite warning */
        s_WarningMissingSpriteTexture.reset( Texture2D::Create(MISSING_SPRITE_TEXTURE_PATH) );
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
        memset(&(s_Data->stats), 0x00, sizeof(Renderer2DStatistics));
    }
    
    Renderer2DStatistics& Renderer2D::GetStatistics()
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
        /* Max indices reached -> Draw Call + Restart Batch */
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
        /* Max indices reached -> Draw Call + Restart Batch */
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
        /* Max indices reached -> Draw Call + Restart Batch */
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
            /* Max Texture slots reached -> Draw Call + Restart Batch */
            if (s_Data->batchRenderState.textureSlotsIndex >= s_Data->batchRenderState.maxTextureSlots)
            {
                FlushAndReset();    // Resets s_Data->batchRenderState.textureSlotsIndex
            }
            
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
        /* Max indices reached -> Draw Call + Restart Batch */
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
            /* Max Texture slots reached -> Draw Call + Restart Batch */
            if (s_Data->batchRenderState.textureSlotsIndex >= s_Data->batchRenderState.maxTextureSlots)
            {
                FlushAndReset();    // Resets s_Data->batchRenderState.textureSlotsIndex
            }
            
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
    
    // Sprite
    void Renderer2D::DrawQuad(const glm::vec2& position,
                              const glm::vec2& size,
                              const std::shared_ptr<Sprite>& sprite,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, sprite, tilingFactor, tintColor);
    }
    
    // Sprite
    void Renderer2D::DrawQuad(const glm::vec3& position,
                              const glm::vec2& size,
                              const std::shared_ptr<Sprite>& sprite,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {
        const glm::vec2* textureCoords              = sprite->GetTextureCoords();
        const std::shared_ptr<Texture2D> texture    = sprite->GetTexture();
        
        /* Max indices reached -> Draw Call + Restart Batch */
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
            /* Max Texture slots reached -> Draw Call + Restart Batch */
            if (s_Data->batchRenderState.textureSlotsIndex >= s_Data->batchRenderState.maxTextureSlots)
            {
                FlushAndReset();    // Resets s_Data->batchRenderState.textureSlotsIndex
            }
            
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
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
    // Sprite + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position,
                              const glm::vec2& size,
                              float rotation_radians,
                              const std::shared_ptr<Sprite>& sprite,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation_radians, sprite, tilingFactor, tintColor);
    }
    
    // Sprite + Rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position,
                              const glm::vec2& size,
                              float rotation_radians,
                              const std::shared_ptr<Sprite>& sprite,
                              float tilingFactor,
                              const glm::vec4& tintColor)
    {
        const glm::vec2* textureCoords              = sprite->GetTextureCoords();
        const std::shared_ptr<Texture2D> texture    = sprite->GetTexture();
        
        /* Max indices reached -> Draw Call + Restart Batch */
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
            /* Max Texture slots reached -> Draw Call + Restart Batch */
            if (s_Data->batchRenderState.textureSlotsIndex >= s_Data->batchRenderState.maxTextureSlots)
            {
                FlushAndReset();    // Resets s_Data->batchRenderState.textureSlotsIndex
            }
            
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
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[0];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 1
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[1];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 2
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[2];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        // 3
        s_Data->batchRenderState.quadVertexBuffer_Ptr->position     = transform * s_Data->batchRenderState.quadVertexPositions[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->color        = tintColor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texCoord     = textureCoords[3];
        s_Data->batchRenderState.quadVertexBuffer_Ptr->texIndex     = textureIndex;
        s_Data->batchRenderState.quadVertexBuffer_Ptr->tilingFactor = tilingFactor;
        s_Data->batchRenderState.quadVertexBuffer_Ptr++;
        
        s_Data->batchRenderState.indicesCounter += s_Data->batchRenderState.indicesPerQuad;
        
        /* Update stats */
        s_Data->stats.quadCount++;
    }
    
}