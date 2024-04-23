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

#include "StandaloneApp.h"
#include <coconuts/Logger.h>
#include <coconuts/types.h>
#include <coconuts/FileSystem.h>


int main(int argc, char* argv[])
{
    Coconuts::StandaloneApp app( (std::string(argv[0])) );
    app.Start();
    return 0;
}

namespace Coconuts
{

    StandaloneApp::StandaloneApp(const std::string& appname)
    :   p_EditorWindow(), p_EditorGUILayer(), p_GameApp(), m_IsRunning(false)
    {
        Coconuts::Logger::Init();

        if( !Coconuts::FileSystem::Boot(appname.c_str()) )
        {
            LOG_ERROR("FileSystem Tools failed to initialize!");
        }

        // Initialize Editor Application Window
        p_EditorWindow.reset( Window::Create( WindowProperties("Coconuts") ) );

        if ( p_EditorWindow->InitWindowManagerCallbacks("GLFW") )
        {
            LOG_DEBUG("Window Manager library callbacks successfully initialized");
        }
        else
        {
            LOG_ERROR("Failed to initialize Window Manager library callbacks!");
        }
        p_EditorWindow->SetEventCallback(BIND_EVENT_FUNCTION(StandaloneApp::OnEvent));
        p_GameApp = std::unique_ptr<Application>(new Application(p_EditorWindow, "Editor"));

        p_EditorGUILayer = std::unique_ptr<EditorLayer>(new EditorLayer(*p_EditorWindow));
        p_EditorGUILayer->Init();

        LOG_INFO("Editor App created");
    }

    StandaloneApp::~StandaloneApp()
    {
    }

    void StandaloneApp::Start(void)
    {
        m_IsRunning = true;

        while(m_IsRunning)
        {
            p_EditorGUILayer->Draw();   // Draw Editor GUI
            p_GameApp->Run();           // Game Loop (1 frame)
            p_EditorWindow->OnUpdate(); // Refresh window
        }
    }

    void StandaloneApp::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        
        // Window Close
        dispatcher.Dispatch<WindowEvent::WindowClose>(BIND_EVENT_FUNCTION(StandaloneApp::OnWindowClose));
        
        // Window Resize
        dispatcher.Dispatch<WindowEvent::WindowResize>(BIND_EVENT_FUNCTION(StandaloneApp::OnWindowResize));
        
        // Window Minimize
        dispatcher.Dispatch<WindowEvent::WindowMinimize>(BIND_EVENT_FUNCTION(StandaloneApp::OnWindowMinimize));
    }

    bool StandaloneApp::OnWindowClose(WindowEvent::WindowClose& event)
    {
        LOG_TRACE("OnWindowClose");
        m_IsRunning = false;
        return true;
    }

    bool StandaloneApp::OnWindowResize(WindowEvent::WindowResize& event)
    {
        LOG_TRACE("OnWindowResize");
        return true;
    }

    bool StandaloneApp::OnWindowMinimize(WindowEvent::WindowMinimize& event)
    {
        LOG_TRACE("OnWindowMinimize");
        return true;
    }

}