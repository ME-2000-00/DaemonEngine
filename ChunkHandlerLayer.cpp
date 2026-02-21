#include "ChunkHandlerLayer.h"


#include <imgui.h>
#include "World.h"



void ChunkHandlerLayer::onAttach() {
    // Runs once when the layer is added
}

void ChunkHandlerLayer::update() {
    // Per-frame logic (animations, state, etc.)
    world.update();
}

void ChunkHandlerLayer::render() {
    // ImGui window
    if (showWindow) {
        ImGui::Begin("Chunk Handler", &showWindow);
        ImGui::Text("Chunks %d" + world.chunks.size());
        ImGui::End();
    }

        
    // lock for safe access
    {
        std::lock_guard<std::recursive_mutex> lock(world.ChunkMutex);

        for (auto& [pos, chunk] : world.chunks) {
            chunk->render();
        }
    }
}

void ChunkHandlerLayer::onMenuBar() {
    if (ImGui::BeginMenu("Chunk Handler")) {
        ImGui::MenuItem("Show Window", nullptr, &showWindow);
        ImGui::EndMenu();
    }
}


void ChunkHandlerLayer::onDetach() {
    for (auto& [pos, chunk] : world.chunks) {
        chunk->cleanup();
    }

    world.exit();
}

LayerKind ChunkHandlerLayer::getKind() {
    return LayerKind::Gui;
}
