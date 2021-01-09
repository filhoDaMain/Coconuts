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

namespace Coconuts
{
    
    struct OrthoCameraComponent
    {
        //data
        float               aspectRatio;
        float               zoomLevel;
        float               mooveSpeed;
        OrthographicCamera  camera;
        //CameraController    controller;
        
        OrthoCameraComponent() = default;
        OrthoCameraComponent(float aR, float zoom)
        :   aspectRatio(aR),
            zoomLevel(zoom),
            camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel)
            //controller(camera, aspectRatio, zoomLevel)
        {}
    };
    
}

#endif /* ORTHOCAMERACOMPONENT_H */

