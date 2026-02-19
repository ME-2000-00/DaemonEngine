#pragma once
#include "LayerRequest.h"
#include "Layer.h"
#include "camera.h"

class ChunkHandlerLayerRequest : public LayerRequest {
public:
	void execute(const RequestContext& RCTX) override;
	Camera* user_cam;
};
