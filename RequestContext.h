#pragma once
#include <iostream>
#include <vector>
#include <memory>

class Layer;



class RequestContext {
public :
	std::vector<Layer*> layers;


	std::vector<Layer*>& get_layers() {
		return layers;
	}
};