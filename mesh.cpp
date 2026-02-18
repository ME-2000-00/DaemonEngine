#include "mesh.h"



#include <imgui.h>
#include <GL/glew.h>
#include "math_helpers.h"




void Mesh::onAttach() {
    // Runs once when the layer is added


    // some random stuff for the beningging
    color.r = MathHelpers::Random01(0.0, 1.0);
    color.g = MathHelpers::Random01(0.0, 1.0);
    color.b = MathHelpers::Random01(0.0, 1.0);
}

void Mesh::update() {
    // Per-frame logic (animations, state, etc.)
}

void Mesh::render() {
    // legacy opengl rect 

    glColor3f(color.r, color.g, color.b);

    glBegin(GL_QUADS);
    glVertex2f(      offsetX - 0.5f,   1 + offsetY - 0.5f);
    glVertex2f(  1 + offsetX - 0.5f,   1 + offsetY - 0.5f);
    glVertex2f(  1 + offsetX - 0.5f,       offsetY - 0.5f);
    glVertex2f(      offsetX - 0.5f,       offsetY - 0.5f);

    glEnd();


}

void Mesh::onMenuBar() {

}


void Mesh::onDetach() {

}

LayerKind Mesh::getKind() {
    return LayerKind::Mesh;
}