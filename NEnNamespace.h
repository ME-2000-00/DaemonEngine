#pragma once
#include "imgui_impl_glfw.h"
#include <glm.hpp>
#include "camera.h"
#include <array>

namespace NEngine {
	inline GLFWwindow* window;
	inline glm::vec2 window_res = glm::vec2(-1, -1);

	inline glm::vec3 light = glm::vec3(0.4, 0.8, -0.1);
	inline glm::vec3 tint = glm::vec3(.2, .4, .37);

	inline std::array<glm::vec3, 3> colors;

	inline Camera user_cam;

	inline bool running = true;

	inline bool destroy = false;
	inline bool build = false;

	inline glm::vec2 mouse_pos = glm::vec2(0, 0);
	inline glm::vec2 rel_mouse_pos = glm::vec2(0, 0);
	inline glm::vec3 input_vector = glm::vec3(0, 0, 0);
}