#pragma once
#include <iostream>
#include <vector>
#include <memory>

class Layer;
class Camera;


class RequestContext {
public :
	std::vector<Layer*> layers;
	Camera* cam;

	std::vector<Layer*>& get_layers() {
		return layers;
	}
};