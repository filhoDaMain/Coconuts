#include "EditorLayer.h"


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