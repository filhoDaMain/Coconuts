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
#ifndef OPENGLGRAPHICSCONTEXT_H
#define OPENGLGRAPHICSCONTEXT_H

#include <coconuts/graphics/GraphicsContext.h>
#include <GLFW/glfw3.h>

namespace Coconuts
{
    
    enum TargetPlatform
    {
        Platform_None   = 0,
        Platform_MacOS  = 1,
        Platform_GNU    = 2
    };
    
    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow* window);
        
        static void PreInitHints(unsigned int platform);
        void Init() override;
        void SwapBuffers() override;
        
    private:
        GLFWwindow* windowHandle;
    };
    
}

#endif /* OPENGLGRAPHICSCONTEXT_H */

