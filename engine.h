#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "Layer.h"
#include "Logger.h"

#include "RequestHandler.h"


namespace NEngine {
	inline GLFWwindow* window;
	inline glm::vec2 window_res = glm::vec2(-1, -1);

	inline Camera user_cam;

	inline bool running = true;

	inline glm::vec2 mouse_pos = glm::vec2(0, 0);
	inline glm::vec2 rel_mouse_pos = glm::vec2(0, 0);
	inline glm::vec3 input_vector = glm::vec3(0, 0, 0);

}



// main engine class
class Engine {
public:
	void init(bool debug_on_start);
	void render();
	void update();
	//void layer_events();
	void exit();

	void pushLayer(std::unique_ptr<Layer> layer);
	void handleLayerPacketQue();
	void populateRCTX();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	const std::vector<std::unique_ptr<Layer>>& get_layers() {
		return layers;
	}

private :
	std::vector<std::unique_ptr<Layer>> layers;
	LayerRequestHandler LRH;
	RequestContext RCTX;
	bool debug_mode = false;
	float dt = 0.0f;
	float cam_speed = 40.0f;
	float fov = 120.0f;
};
