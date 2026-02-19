#include "ChunkHandlerLayerRequest.h"
#include "Logger.h"

#include "RequestContext.h"
#include "ChunkHandlerLayer.h"


void ChunkHandlerLayerRequest::execute(const RequestContext& RCTX) {
	Layer* sender = getSender();

	// if sender is inspector layer
	if (sender && sender->getKind() == LayerKind::Gui) {
		// handle in spector stuff
		auto* layer = static_cast<ChunkHandlerLayer>(sender);

		layer->user_cam = RCTX.cam;

	}

}