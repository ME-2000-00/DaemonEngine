#pragma once

#include "Layer.h"


class TestLayer : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;
private:
    float clearColor[3] = { 0.2f, 0.3f, 0.3f };
    bool showWindow = true;
};