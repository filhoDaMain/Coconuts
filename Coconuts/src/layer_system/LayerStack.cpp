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

#include <coconuts/Layer.h>
#include <coconuts/layer_system/LayerStack.h>

namespace Coconuts
{
    /**
     *  Stack structure 
     *  Overview
     *      
     *       Further away
     *      +-----------+
     *   ^  |   Layer   |   0
     *   |  +-----------+
     *   |  |   Layer   |   1
     *   |  +-----------+
     *   |  |   Layer   |   2   =   i_LayerInsert
     *   |  +-----------+ <-------------------------  Next Layer will be pushed here
     *   |  |  Overlay  |
     *   |  +-----------+
     *   |  |  Overlay  |
     *   |  +-----------+
     *   |    TOP VIEW
     *   |
     *  Screen View
     *
     */
    
    
    LayerStack::LayerStack()
    {
        i_LayerInsert = v_Layers.begin();
    }
    
    LayerStack::~LayerStack()
    {
        for (Layer* layer : v_Layers)
        {
            delete layer;
        }
    }
    
    void LayerStack::PushLayer(Layer* layer)
    {
        i_LayerInsert = v_Layers.emplace(i_LayerInsert /* Last Layer position */,
                                         layer);
    }
    
    void LayerStack::PushOverlay(Layer* overlay)
    {
        v_Layers.emplace_back(overlay);
    }
    
    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(v_Layers.begin(), v_Layers.end(), layer);
        
        /* Found */
        if (it != v_Layers.end())
        {
            v_Layers.erase(it);
            i_LayerInsert--;
        }
    }
    
    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(v_Layers.begin(), v_Layers.end(), overlay);
        
        /* Found */
        if (it != v_Layers.end())
        {
            v_Layers.erase(it);
        }
    }
}