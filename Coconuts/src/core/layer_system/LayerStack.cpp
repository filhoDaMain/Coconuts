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
#include <algorithm>

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
    : m_Layers()
    {
        m_LayerInsert = m_Layers.begin();
    }

    void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert /* Last Layer position */,
                                         layer);
    }

    void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay)
    {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        
        /* Found */
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsert--;
        }
    }

    void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        
        /* Found */
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }
}