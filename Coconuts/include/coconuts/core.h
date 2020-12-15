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

#ifndef COCONUTS_CORE_H
#define COCONUTS_CORE_H

#include <coconuts/Application.h>
#include <coconuts/Logger.h>

#define COCONUTS_VERSION 0.1F

/* core.cpp */
extern void coconuts_version(void);


/* Entry Point for Standalone Editor */
#ifdef __COCONUTS_STANDALONE_EDITOR_APP__
int main (void)
{
    Coconuts::Logger::Init();
    auto app = Coconuts::CreateApplication();
    app->Run();
    delete app;
    
    LOG_TRACE("Closing Editor window");
    LOG_WARN("Goodbye!");
    return 0;
}
#endif

/* Entry Point for Sandbox applications */
#ifdef __COCONUTS_SANDBOX_APP__
int main (void)
{
    Coconuts::Logger::Init();
    auto app = Coconuts::CreateApplication();
    app->Run();
    delete app;
    
    LOG_TRACE("All instances of Sandbox Application died");
    LOG_WARN("Goodbye!");
    return 0;
}
#endif


#endif