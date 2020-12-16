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
#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include <glm/glm.hpp>

namespace Coconuts
{
    
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        
        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline float GetRotation() const { return m_Rotation; }
        
        void SetProjection(float left, float right, float bottom, float top);
        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
        
        inline const glm::mat4& GetProjMatrix() const { return m_ProjMatrix; }
        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        inline const glm::mat4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
        
    private:
        void RecalculateViewMatrix();
        
    private:
        float m_AspectRatio;
        
        glm::mat4 m_ProjMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjMatrix;
        
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        float m_Rotation = 0.0f;
    };
    
}

#endif /* ORTHOGRAPHICCAMERA_H */

