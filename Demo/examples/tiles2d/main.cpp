/**
 * Must define __COCONUTS_SANDBOX_APP__
 * to explicitly define this module
 * as the Entry Point
 *  
 */
#ifndef __COCONUTS_SANDBOX_APP__
#define __COCONUTS_SANDBOX_APP__
#endif  // __COCONUTS_SANDBOX_APP__

#include <coconuts/core.h>
#include "MainApp.h"

Coconuts::Application* Coconuts::CreateApplication()
{
    return new MainApp();
}

