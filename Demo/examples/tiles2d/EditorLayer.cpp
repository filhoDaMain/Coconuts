#include "EditorLayer.h"
#include "GameLayer.h"
#include <cstdint>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void EditorLayer::OnUpdate(Coconuts::Timestep ts)
{    
    /* Get Live statistics */
    stats = Coconuts::Renderer2D::GetStatistics();    
    
    ImGui::Begin("Statistics");
    ImGui::Text("Batch render statistics:");
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();    
    ImGui::Text("%d Draw Calls", stats.drawCalls);
    ImGui::Spacing();
    ImGui::Text("%d Quads", stats.quadCount);
    ImGui::End();
}

EditorLayer::EditorLayer()
{
    
}