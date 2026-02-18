#pragma once





#include "Layer.h"
#include "Chunk_1DArray.h"



class ChunkHandlerLayer : public Layer {
public:
    void onAttach() override;
    void update() override;
    void render() override;
    void onMenuBar() override;

private:
    std::vector<Chunk_1DArray> chunks;
    bool showWindow = false;
    World world;

};

