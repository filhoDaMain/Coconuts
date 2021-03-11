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
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glm/glm.hpp>
#include <memory>
#include <coconuts/graphics/VertexArray.h>
#include <coconuts/graphics/Shader.h>
#include <coconuts/graphics/Texture.h>
#include <coconuts/graphics/Sprite.h>

/* Cameras */
#include <coconuts/cameras/OrthographicCamera.h>

namespace Coconuts
{
    
    struct QuadVertex
    {
        glm::vec3   position;       /* vertex xyz position */
        glm::vec4   color;
        glm::vec2   texCoord;       /* texture coordinate to match with the vertex position */
        float       texIndex;       /* Index/ID of the texture */
        float       tilingFactor;   /* tiling */
    };
    
    struct BatchRender
    {
        /* Per Draw call (batched) constants */
        static const uint32_t maxQuads          = 10000;
        static const uint32_t verticesPerQuad   = 4;
        static const uint32_t indicesPerQuad    = 6;
        static const uint32_t maxVertices       = maxQuads * verticesPerQuad;
        static const uint32_t maxIndices        = maxQuads * indicesPerQuad;
        static const uint32_t maxTextureSlots   = 16;   // GPU dependent (defined in frag shader)
        
        /* 
         * When indicesCounter > maxIndices, Renderer is flushed
         * and a new Draw call is issued
         */
        uint32_t indicesCounter = 0;
        
        /* Texture Slots */
        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots; 
        const uint32_t minTextureSlotIndex  = 1; // '0' os reserved for blank/default texture
        /* Init slot index counter */
        uint32_t textureSlotsIndex = minTextureSlotIndex;
      
        QuadVertex* quadVertexBuffer_Base   = nullptr;
        QuadVertex* quadVertexBuffer_Ptr    = nullptr;
        glm::vec4 quadVertexPositions[4];
    };
    
    struct Renderer2DStatistics
    {
        uint32_t drawCalls  = 0;
        uint32_t quadCount  = 0;
        
        uint32_t GetTotalVertexCount() { return quadCount * BatchRender::verticesPerQuad; }
        uint32_t GetTotalIndexCount() { return quadCount * BatchRender::indicesPerQuad; }
    };
    
    struct Renderer2DStorage
    {
        BatchRender             batchRenderState;
        Renderer2DStatistics    stats;
        
        std::shared_ptr<VertexArray>    vertexArray;
        std::shared_ptr<VertexBuffer>   vertexBuffer;
        std::shared_ptr<Shader>         shader;
        std::shared_ptr<Texture2D>      texture2D_Blank;
    };
    
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();
        
        static void ResetStatistics();
        static Renderer2DStatistics& GetStatistics();
        
        /* Flat Colors */
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation_radians, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation_radians, const glm::vec4& color);
        
        /* Texture Images */
        static void DrawQuad(const glm::vec2& position,
                             const glm::vec2& size,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawQuad(const glm::vec3& position,
                             const glm::vec2& size,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec2& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec3& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        /* Sprites */
        static void DrawQuad(const glm::vec2& position,
                             const glm::vec2& size,
                             const std::shared_ptr<Sprite>& sprite,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawQuad(const glm::vec3& position,
                             const glm::vec2& size,
                             const std::shared_ptr<Sprite>& sprite,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec2& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Sprite>& sprite,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec3& position,
                             const glm::vec2& size,
                             float rotation_radians,
                             const std::shared_ptr<Sprite>& sprite,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        
        
        static std::unique_ptr<Texture2D>& GetDefaultMissingSpriteTexture() { return s_WarningMissingSpriteTexture; }
        
    private:
        static void FlushAndReset();
        
    private:
        static std::unique_ptr<Texture2D> s_WarningMissingSpriteTexture;
    };
    
}

#endif /* RENDERER2D_H */

