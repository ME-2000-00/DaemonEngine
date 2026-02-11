#pragma once


#include "Layer.h"


class Inspecter : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;
    void onDetach() override;
    LayerKind getKind() override;

    std::vector<Layer*> layers;
private:
    bool log = false;
};