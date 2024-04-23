/*
 * Copyright 2024 Andre Temprilho
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

#ifndef STANDALONE_APP_H
#define STANDALONE_APP_H

#include <coconuts/window_system/Window.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Application.h>
#include <thread>
#include "EditorLayer.h"

namespace Coconuts
{

    class StandaloneApp
    {
    public:
        StandaloneApp(const std::string& appname);
        ~StandaloneApp();

        void Start(void);
    
    private:
        void OnEvent(Event& event);
        bool OnWindowClose(WindowEvent::WindowClose& event);
        bool OnWindowResize(WindowEvent::WindowResize& event);
        bool OnWindowMinimize(WindowEvent::WindowMinimize& event);
    
    private:
        std::shared_ptr<Window> p_EditorWindow;
        std::unique_ptr<EditorLayer> p_EditorGUILayer;
        std::unique_ptr<Application> p_GameApp;
        bool m_IsRunning;
    };
}

#endif