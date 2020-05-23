/**
 * A Sandbox Application example to show Coconuts running.
 */

//!!!
/* Must define __COCONUTS_SANDBOX_APP__ prior to any Coconuts include */
#ifndef __COCONUTS_SANDBOX_APP__
#define __COCONUTS_SANDBOX_APP__
#endif  // __COCONUTS_SANDBOX_APP__
//!!!

#include <coconuts/core.h>
#include <coconuts/Application.h>
#include <coconuts/Layer.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Logger.h>

#include <coconuts/editor.h>

#include <string>
#include "demo.h"


/**
 * Define a new custom Layer, inherited from Coconuts::Layer base class.
 * 
 * Event triggers and draw calls are issued per Layer.
 * Overlays get always on top of the last inserted Layer.
 * 
 * The rendering order of Layers/Overlays is as follows:
 * 
 *      Further away
 *  +-----------+
 *  |   Layer   |   0
 *  |   +-----------+
 *  +---|   Layer   |   1
 *      |   +-----------+
 *      +---|   Layer   |   2
 *          |   +-----------+ <----------- Next Layer will be pushed here
 *          +---|  Overlay  |   3
 *              |   +-----------+
 *              +---|  Overlay  |
 *                  |           |   4
 *                  +-----------+ <------- Next Overlay will be pushed here
 *      TOP VIEW
 */
class ExampleLayer : public ::Coconuts::Layer
{
    public:
        ExampleLayer(const std::string& layerName)  : Layer(layerName)
        {
            // inits and allocations
        }
        
        /**
         * Called once per frame
         */
        void OnUpdate() override
        {
            //TODO
        }
        
        /**
         * Triggered whenever an Event occurs
         */
        void OnEvent(Coconuts::Event& event) override
        {
            /* Log the event */
            //LOG_TRACE(event.ToString());
        }
};


/**
 * Define a new Application, inherited from Coconuts::Application base clase
 */
class DemoApp : public ::Coconuts::Application
{
public:
    DemoApp()
    {
        /**
         * Create a new Layer and push it to the Layer Stack
         * of 'this' Application
         */
        ExampleLayer* newLayer = new ExampleLayer("Layer0");
        this->PushLayer(newLayer);
        
        //GUI
        /**
         * Create a new GUI Overlay and push it to the Overlay Stack
         * of 'this' Application
         */
        ::Coconuts::Editor::GUILayer* guiOverlay = new ::Coconuts::Editor::GUILayer();
        this->PushOverlay(guiOverlay);
        
    }
    
    ~DemoApp()
    {
        
    }
};


/**
 * Feed Coconuts with a new user defined Application that
 * inherits from Coconuts::Application base class
 */
Coconuts::Application* Coconuts::CreateApplication()
{
    return new DemoApp();
}