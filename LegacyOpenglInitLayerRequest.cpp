#include "LegacyOpenglInitLayerRequest.h"
#include "Logger.h"

#include "RequestContext.h"
#include "LegacyOpenglInitLayer.h"


void LegacyOpenglInitLayerRequest::execute(const RequestContext& RCTX) {
	Layer* sender = getSender();

	// if sender is inspector layer
	if (sender && sender->getKind() == LayerKind::Gui) {
		// handle in spector stuff
		auto* legacyOpenglInitLayer = static_cast<LegacyOpenglInitLayer*>(sender);

		legacyOpenglInitLayer->user_cam = RCTX.cam;

	}

}