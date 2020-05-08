/**
 * A Sandbox Application example to show Coconuts running.
 */

//!!!
/* Must define __COCONUTS_SANDBOX_APP__ prior to any Coconuts include */
#ifndef __COCONUTS_SANDBOX_APP__
#define __COCONUTS_SANDBOX_APP__
#endif

#include <coconuts/core.h>
#include <coconuts/Logger.h>


#include <iostream>
#include "demo.h"


namespace Coconuts
{
    /**
     * Create a new Sandbox Application.
     * Check Coconuts running state output in the terminal. 
     */
    Application* CreateApplication()
    {
        return new Application();
    }
}