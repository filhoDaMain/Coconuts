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
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>
#include <coconuts/EventSystem.h>

namespace Coconuts
{
    struct WindowProperties
    {
        std::string m_Title;
        unsigned int m_Width;
        unsigned int  m_Height;
        
        WindowProperties(const std::string& title  = "Coconuts" /* Default name */,
                         unsigned int width = 1280, unsigned int height = 720)
                : m_Title(title), m_Width(width), m_Height(height)
        {
            //do nothing
        }
    };
    
    class Window
    {
    public:
        using EventCallbackFunction = std::function<void(Event& event)>;
        
        virtual ~Window() = default;
        
        /* Called once per frame */
        virtual void OnUpdate() = 0;
        
        /* MUST be implemented by a platform dependent sub Window class */
        virtual void SetEventCallback(const EventCallbackFunction& callbackFn) = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        
        static Window* Create(const WindowProperties& props = WindowProperties());
        
        
    private:
    };
    
}

#endif /* WINDOW_H */

