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
#ifndef ORTHOCAMERACOMPONENT_H
#define ORTHOCAMERACOMPONENT_H

#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/cameras/CameraController.h>
#include <glm/glm.hpp>

namespace Coconuts
{
    
    struct OrthoCameraComponent
    {
        //data
        float               aspectRatio;
        float               zoomLevel;
        float               mooveSpeed;
        bool                halt;
        glm::vec4           backgroundColor;
        OrthographicCamera  camera;
        
        OrthoCameraComponent()
        :   aspectRatio(16.0f/9.0f),
            zoomLevel(1.0f),
            mooveSpeed(1.0f),
            halt(false),
            backgroundColor({0.0f, 0.0f, 0.0f, 1.0f}),
            camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel)
        {}
        
        OrthoCameraComponent(float aR, float zoom, const glm::vec4& bgColor = {0.0f, 0.0f, 0.0f, 1.0f})
        :   aspectRatio(aR),
            zoomLevel(zoom),
            halt(false),
            backgroundColor(bgColor),
            camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel)
        {}
    };
    
}

#endif /* ORTHOCAMERACOMPONENT_H */

