#pragma once

#include "Layer.h"
#include <glm.hpp>


class Mesh : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;
    void onDetach() override;
    LayerKind getKind() override;
    glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);

    int offsetX = 0;
    int offsetY = 0;


private:
    // add layer attribs here later
    // have data layer that is the mesh object
    // will for now just be a legacy opengl rect in the render function
};