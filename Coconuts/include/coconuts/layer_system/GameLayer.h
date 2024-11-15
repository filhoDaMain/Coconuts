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
#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <memory>
#include <coconuts/Renderer.h>
#include <coconuts/Layer.h>
#include <coconuts/ECS.h>
#include <vector>

namespace Coconuts
{
    
    class GameLayer : public Layer
    {
        private:
            /* Entity handlers */
            Entity m_Entity;

        public:
            GameLayer();
            virtual ~GameLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(Timestep ts) override;
            virtual void OnEvent(Event& event) override;
            
            /**
             * Halts and re-establishes event handling on this layer.
             * 
             * Input:
             * state    - true:  Halts Event Handling. This layer will not respond to any events.
             *          - false: Re-establishes event handling. This layer responds to its events.
             * 
             * Returns: current halt state after change.
             */
            bool HaltEvents(bool state = true);
            
            /* Change on viewport notification */
            void ChangeViewport(float x, float y);
            
            bool IsActiveSceneUpdated();
            std::vector<Entity> GetActiveSceneEntities() const;
            
            /* Create New Empty Entity on current active Scene */
            bool NewEntity();
            
            /* Destroy Entity */
            bool DestroyEntity(uint64_t id);
            
            /* Get Active Scene's aspect ratio */
            float GetActiveSceneAR();
            
            // --------------------------------------
            // Debug Color change
            Entity& GetEntity() {return m_Entity; }
            // --------------------------------------
    };
    
}

#endif /* GAMELAYER_H */

