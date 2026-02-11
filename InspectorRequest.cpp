#include "InspectorRequest.h"
#include "Logger.h"

#include "RequestContext.h"
#include "Inspecter.h"


void InspectorRequest::execute(const RequestContext& RCTX) {
	Layer* sender = getSender();
	mesh_layers.clear();

	for (auto& layer : RCTX.layers) {
		if (layer->getKind() == LayerKind::Mesh) {
			mesh_layers.push_back(layer);
		}
	}


	// if sender is inspector layer
	if (sender && sender->getKind() == LayerKind::Inspector) {
		// handle in spector stuff
		auto* inspecter = static_cast<Inspecter*>(sender);
		
		inspecter->layers = mesh_layers;

	}

}