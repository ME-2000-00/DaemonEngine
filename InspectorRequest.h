#pragma once

#include "LayerRequest.h"
#include "Layer.h"


class InspectorRequest : public LayerRequest {
public:
	void execute(const RequestContext& RCTX) override;
	std::vector<Layer*> mesh_layers;
};
