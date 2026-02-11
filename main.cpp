#include "engine.h"
#include <iostream>




int main(void) {
	Engine engine = Engine();



	engine.init(false);

	while (NEngine::running && !glfwWindowShouldClose(NEngine::window)) {

		// first give layers the data they need 
		engine.handleLayerPacketQue();

		// then render and update layers
		engine.render();
		engine.update();
	};

	engine.exit();


	return 0;
}