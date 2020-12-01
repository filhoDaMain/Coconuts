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

/* Cameras */
#include <coconuts/cameras/OrthographicCamera.h>

namespace Coconuts
{
    
    struct Renderer2DStorage
    {
        std::shared_ptr<VertexArray>    vertexArray_Quad;
        std::shared_ptr<Shader>         shader_Texture;
        std::shared_ptr<Texture2D>      texture2D_Blank; 
    };
    
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        
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
    };
    
}

#endif /* RENDERER2D_H */

