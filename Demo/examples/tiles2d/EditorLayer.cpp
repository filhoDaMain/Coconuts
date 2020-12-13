#include "EditorLayer.h"
#include "GameLayer.h"
#include <cstdint>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void EditorLayer::OnUpdate(Coconuts::Timestep ts)
{
#if 1
    /* Get Live statistics */
    stats = Coconuts::Renderer2D::GetStatistics();
    
    ImGui::Begin("Statistics");
    ImGui::Text("Batch render statistics:");
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    ImGui::Text("%d Draw Calls", stats.drawCalls);
    ImGui::Spacing();
    ImGui::Text("%d Quads", stats.quadCount);
    ImGui::End();
#endif
    
    uint32_t viewPortTexID = GameLayer::GetFramebuffer()->GetColorAttachID();
    
    ImGui::Begin("ViewPort");
    ImGui::Image(INT2VOIDP(viewPortTexID), ImVec2{320.0f, 174.0f});
    ImGui::End();
}

EditorLayer::EditorLayer()
{
    
}