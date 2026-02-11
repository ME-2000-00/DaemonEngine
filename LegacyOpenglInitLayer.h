#pragma once

#include "Layer.h"


class LegacyOpenglInitLayer : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;
    void onDetach() override;
    LayerKind getKind() override;


private:
    bool show_keyboard_window = false;
    bool logger = false;
};