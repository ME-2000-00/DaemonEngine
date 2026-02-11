#include "Inspecter.h"

#include <imgui.h>
#include <GL/glew.h>
#include "InspectorRequest.h"
#include "Logger.h"
#include "mesh.h"


void Inspecter::onAttach() {
    // Runs once when the layer is added
    addRequest(std::make_unique<InspectorRequest>());
}

void Inspecter::update() {
    // Per-frame logic (animations, state, etc.)
    addRequest(std::make_unique<InspectorRequest>());
}

void Inspecter::render() {
    // imgui window

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 size = viewport->Size;

    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(400, size.y - 20));

    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        for (const auto& layer : layers)
        {
            auto* mesh = static_cast<Mesh*>(layer);

            ImGui::Text("Color: r=%.2f g=%.2f b=%.2f", mesh->color.r, mesh->color.g, mesh->color.b);
        }
    }
    ImGui::End();
}

void Inspecter::onMenuBar() {

}

void Inspecter::onDetach() {

}

LayerKind Inspecter::getKind() {
    return LayerKind::Inspector;
}