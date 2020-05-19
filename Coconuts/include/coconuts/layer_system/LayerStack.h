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
        ~LayerStack();
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);
        
        std::vector<Layer*>::iterator begin() { return v_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return v_Layers.end(); }
        
    private:
        std::vector<Layer*> v_Layers;   /* Contuguous heap storage of raw vectors */
        std::vector<Layer*>::iterator i_LayerInsert;
    };
}

#endif /* LAYERSTACK_H */

