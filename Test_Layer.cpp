#include "Test_Layer.h"

#include <imgui.h>
#include <GL/glew.h>





void TestLayer::onAttach() {
    // Runs once when the layer is added
}

void TestLayer::update() {
    // Per-frame logic (animations, state, etc.)
}

void TestLayer::render() {
    // ImGui window
    if (showWindow) {
        ImGui::Begin("Test Layer", &showWindow);
        ImGui::Text("Hello from TestLayer");
        ImGui::ColorEdit3("Clear Color", clearColor);
        ImGui::End();
    }

    // You could also do OpenGL rendering here
    glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
}

void TestLayer::onMenuBar() {
    if (ImGui::BeginMenu("Test")) {
        ImGui::MenuItem("Show Window", nullptr, &showWindow);
        ImGui::EndMenu();
    }
}