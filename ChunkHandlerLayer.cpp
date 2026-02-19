#include "ChunkHandlerLayer.h"


#include <imgui.h>
#include "World.h"



void ChunkHandlerLayer::onAttach() {
    // Runs once when the layer is added
    world.genStartChunks();
}

void ChunkHandlerLayer::update() {
    // Per-frame logic (animations, state, etc.)
    world.update();
}

void ChunkHandlerLayer::render() {
    // ImGui window
    if (showWindow) {
        ImGui::Begin("Chunk Handler", &showWindow);
        ImGui::Text("Chunk Amount: %d", world.chunks.size());
        ImGui::Text("Steps: %d", world.player.points.size());
        ImGui::End();
    }

        
	for (auto& [pos, chunk] : world.chunks) {
        if (chunk->ignore) {
            continue;
		}
        chunk->render();
    }

    world.render();
}

void ChunkHandlerLayer::onMenuBar() {
    if (ImGui::BeginMenu("Chunk Handler")) {
        ImGui::MenuItem("Show Window", nullptr, &showWindow);
        ImGui::EndMenu();
    }
}


void ChunkHandlerLayer::onDetach() {

}

LayerKind ChunkHandlerLayer::getKind() {
    return LayerKind::Gui;
}
