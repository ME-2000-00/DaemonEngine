#include "RequestHandler.h"
#include "Layer.h"         // full definition of Layer
#include "LayerRequest.h"  // full definition of LayerRequest
#include "RequestContext.h"



// handles all Requests send from layers
void LayerRequestHandler::handleRequests(RequestContext& RCTX) {
	
	for (auto& layer : RCTX.get_layers()) {
		for (auto& req : layer->requests) {
			req->execute(RCTX);
		}

		layer->requests.clear();
	}

}