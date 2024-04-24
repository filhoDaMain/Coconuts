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
#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <coconuts/Layer.h>
#include <vector>

namespace Coconuts
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack() = default;
        
        void PushLayer(std::shared_ptr<Layer> layer);
        void PushOverlay(std::shared_ptr<Layer> overlay);
        
        void PopLayer(std::shared_ptr<Layer> layer);
        void PopOverlay(std::shared_ptr<Layer> overlay);
        
        std::vector< std::shared_ptr<Layer> >::iterator begin() { return m_Layers.begin(); }
        std::vector< std::shared_ptr<Layer> >::iterator end() { return m_Layers.end(); }
        
    private:
        std::vector< std::shared_ptr<Layer> > m_Layers;
        std::vector< std::shared_ptr<Layer> >::iterator m_LayerInsert;

    };
}

#endif /* LAYERSTACK_H */

