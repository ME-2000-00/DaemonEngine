#pragma once

#include "Layer.h"



class InputLayer : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onDetach() override;
    void onMenuBar() override;
    LayerKind getKind() override;

private:

};