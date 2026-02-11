#include "mesh.h"



#include <imgui.h>
#include <GL/glew.h>
#include "math_helpers.h"




void Mesh::onAttach() {
    // Runs once when the layer is added

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 size = viewport->Size;
    // some random stuff for the beningging
    color.r = Random01(0.0, 1.0);
    color.g = Random01(0.0, 1.0);
    color.b = Random01(0.0, 1.0);

    offsetX = Random01(0.0, 1.0) * size.x;
    offsetY = Random01(0.0, 1.0) * size.y;
}

void Mesh::update() {
    // Per-frame logic (animations, state, etc.)
}

void Mesh::render() {
    // legacy opengl rect 

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    glm::vec2 size = glm::vec2(viewport->Size.x, viewport->Size.y);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, size.x, size.y, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(color.r, color.g, color.b);

    glBegin(GL_QUADS);
    glVertex2f(      offsetX, 100 + offsetY);
    glVertex2f(100 + offsetX, 100 + offsetY);
    glVertex2f(100 + offsetX,       offsetY);
    glVertex2f(      offsetX,       offsetY);

    glEnd();


}

void Mesh::onMenuBar() {

}


void Mesh::onDetach() {

}

LayerKind Mesh::getKind() {
    return LayerKind::Mesh;
}