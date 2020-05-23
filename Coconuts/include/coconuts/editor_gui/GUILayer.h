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
#ifndef GUILAYER_H
#define GUILAYER_H

#include <coconuts/Layer.h>
#include <coconuts/EventSystem.h>

namespace Coconuts
{
    namespace Editor
    {
        class GUILayer : public Layer
        {
        public:
            GUILayer();
            ~GUILayer();
            
            void OnAttach();
            void OnDetach();
            void OnUpdate();
            void OnEvent(Event& event);
            
        private:
            double m_Time = 0.0f;
        };
    }
}


#endif /* GUILAYER_H */

