#include "engine.h"
#include <iostream>
#include "NEnNamespace.h"


//									-- IMPORTANT --
// change the first argument to true to start in debug mode, false to start in normal mode

int main(int argc, char** argv) {
	Engine engine = Engine();

	// arg 0 is just path of project, arg 1 is whether to start in debug mode or not (true/false)
	bool debug = false;

	if (argc > 1)
	{
		std::string arg = argv[1];
		debug = (arg == "true");
	}

	engine.init(debug);


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