/*
 * Copyright 2021 Andre Temprilho
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

#include "Statistics.h"
#include <coconuts/editor.h>
#include <coconuts/debug/TimeProfiler.h>
#include <coconuts/graphics/Renderer2D.h>

namespace Coconuts {
namespace Panels
{
    
    void Statistics::Draw()
    {
        ImGui::Begin("Statistics");
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        
        bool open = ImGui::TreeNodeEx("Renderer Statistics Summary" , flags);
        
        /* Draw a tree with Arrows */
        if (open)
        {
            // -> Arrow
            std::string fpsKey("Scene:OnUpdate");
            
            Profiler::TimeData fpsData = Profiler::TimeProfiler::GetInstance().Fetch(fpsKey);
            float fps = static_cast<float>(1000000.0f/fpsData.deltaTime_us);    // frames per second
            
            ImGui::Text("Frame Rate: %.1f fps", fps);
            ImGui::Text("Per frame stats:");
            ImGui::Text(" %u Draw calls", Renderer2D::GetStatistics().drawCalls);
            ImGui::Text(" %u Quads", Renderer2D::GetStatistics().quadCount);
            
            ImGui::TreePop();   // Close all
        }
        
        ImGui::End();
    }
    
}
}