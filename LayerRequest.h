#pragma once

class RequestContext; // forward declare
class Layer;

class LayerRequest {
public :
	virtual ~LayerRequest() = default;

	// made it a bool so successful requests can be tracked and be debuged if unsuccsesfull
	virtual void execute(const RequestContext& RCTX) = 0;

	void setSender(Layer* layer) { sender = layer; }
	Layer* getSender() const { return sender; }

protected :
	Layer* sender = nullptr;
};