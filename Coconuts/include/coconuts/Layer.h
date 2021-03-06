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
#ifndef LAYER_H
#define LAYER_H

#include <coconuts/EventSystem.h>
#include <string>
#include <coconuts/time/Timestep.h>

namespace Coconuts
{
    class Layer
    {
    public:
        Layer(const std::string& name = "New Layer");
        virtual ~Layer();
        
        virtual void OnAttach();
        virtual void OnPostAttach();
        virtual void OnDetach();
        virtual void OnUpdate(Timestep ts);
        virtual void OnEvent(Event& event);
        
        inline virtual bool IsGUI() { return false; } 
        
        inline const std::string& GetName() const { return m_Name; }
        
    private:
        std::string m_Name;
    };
}

#endif /* LAYER_H */

