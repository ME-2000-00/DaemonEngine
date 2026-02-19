#pragma once

#include "Chunk_1DArray.h"
#include "Layer.h"
#include "camera.h"




class World;

class ChunkHandlerLayer : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;
    void onDetach() override;
    LayerKind getKind() override;
private:
    std::vector<Chunk_1DArray> chunks;
    bool showWindow = false;
    World world;

};