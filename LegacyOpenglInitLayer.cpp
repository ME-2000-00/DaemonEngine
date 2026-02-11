#include "LegacyOpenglInitLayer.h"

#include <imgui.h>
#include <GL/glew.h>
#include "Logger.h"




void LegacyOpenglInitLayer::onAttach() {
    // Runs once when the layer is added
    //addRequest(std::make_unique<LayerRequest>());
}

void LegacyOpenglInitLayer::update() {
    // Per-frame logic (animations, state, etc.)
}

void LegacyOpenglInitLayer::render() {
    // imgui window

    // keyboard
    if (show_keyboard_window) {
        ImGui::Begin("Keyboard window", &show_keyboard_window);
        ImGui::End();
    }





}

void LegacyOpenglInitLayer::onMenuBar() {
    if (ImGui::BeginMenu("Input Menu")) {
        ImGui::MenuItem("Keys", nullptr, &show_keyboard_window);
        ImGui::MenuItem("Logger", nullptr, &logger);
        ImGui::EndMenu();
    }


    if (logger) {
        // add differnet colors for log types
        static bool autoScroll = true;

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImVec2 size = viewport->Size;


        ImGui::SetNextWindowPos(ImVec2(size.x - 400, 20)); // left side, below menu bar
        ImGui::SetNextWindowSize(ImVec2(400, size.y - 20));

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar;

        if (ImGui::Begin("Engine Log", &logger, flags))
        {
            const auto& logs = Logger::GetMessages();
            for (const auto& msg : logs)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(msg.r, msg.g, msg.b, 255));

                ImGui::TextUnformatted(msg.msg.c_str());
                ImGui::PopStyleColor();
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::Checkbox("Auto-scroll", &autoScroll);
        }

        ImGui::End();
    }
}


void LegacyOpenglInitLayer::onDetach() {

}

LayerKind LegacyOpenglInitLayer::getKind() {
    return LayerKind::Gui;
}
