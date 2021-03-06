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

#include <coconuts/cameras/OrthographicCamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Coconuts
{
    
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
    }
    
    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *\
                                glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
        
        m_ViewMatrix = glm::inverse(transform);
        
        m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
    }
    
    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
    }
    
}