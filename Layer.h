#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "LayerRequest.h"

enum class LayerKind {
	Unknown = 0,
	Mesh,
	Inspector,
	Gui
};





// use  class -classname- : public -other classname (in our case Layer)- to extend Layer (or any class) in another class
class Layer {
public:
	virtual ~Layer() = default;

	virtual void render() {}
	virtual void update() {}
	virtual void events() {}


	virtual void onAttach() {}   // called when added to engine
	virtual void onDetach() {}   // called when removed

	virtual void onMenuBar() {}

	// use for adding new requests
	virtual void addRequest(std::unique_ptr<LayerRequest> request) {

		request->setSender(this);
		requests.push_back(std::move(request));
	}

	void set_id(int new_id) { id = new_id; }
	int get_id() { return id; }

	// shoudl return the layer type
	virtual LayerKind getKind() {
		return LayerKind::Unknown;
	}

	bool use = true;
	std::vector<std::unique_ptr<LayerRequest>> requests;

protected:
	int id = -1;
};

