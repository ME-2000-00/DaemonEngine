#include "engine.h"
#include "Layer.h"
#include "LegacyOpenglInitLayer.h"
#include "Inspecter.h"
#include "Mesh.h"



void Engine::init(bool debug_on_start) {
	// can be set to true on init call to activate debug mode on start of engine
	debug_mode = debug_on_start;


	// init glfw and glew
	// just the docs stuff
    Logger::Log(LogLevel::INFO, "Engine Init. . .");


     /* Initialize the library */
    if (!glfwInit())
        NEngine::running = false;



    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    NEngine::window = glfwCreateWindow(mode->width, mode->height, "DaemonEngine", monitor, NULL);

    if (!NEngine::window)
    {
        glfwTerminate();
        NEngine::running = false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(NEngine::window);

    // Let callbacks access this instance
    glfwSetWindowUserPointer(NEngine::window, this);

    glfwSetKeyCallback(NEngine::window, KeyCallback);
    glfwSetMouseButtonCallback(NEngine::window, MouseCallback);


    if (glewInit() != GLEW_OK) {
        NEngine::running = false;
    }


    int window_width, window_height;
    glfwGetFramebufferSize(NEngine::window, &window_width, &window_height);
    NEngine::user_cam = Camera(glm::vec3(0.0f, 0.0f, -3.0f),    // position
        glm::radians(90.0f),           // FOV in radians
        (float)window_width / window_height,  // aspect ratio
        0.1f, 100.0f);                 // near/far planes


    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // base style



    ImGui_ImplGlfw_InitForOpenGL(NEngine::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // add on start layers here
    pushLayer(std::make_unique<LegacyOpenglInitLayer>());
    //pushLayer(std::make_unique<Inspecter>());




    // Set the clear color (RGBA format, values range from 0.0f to 1.0f)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark cyan
    // enable to capture mouse
    //glfwSetInputMode(NEngine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    Logger::Spacer();
    Logger::Log(LogLevel::INFO, "Engine Initialized");
    Logger::Spacer();

    // didnt work
    // vs cosole is too old for colors ig
    //Logger::Log(LogLevel::INFO, "Testing color INFO");
    //Logger::Log(LogLevel::WARNING, "Testing color WARNING");
    //Logger::Log(LogLevel::ERROR, "Testing color ERROR");
};


void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
    if (!engine) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetInputMode(NEngine::window, GLFW_CURSOR, glfwGetInputMode(NEngine::window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    


}
void Engine::MouseCallback(GLFWwindow* window, int button, int action, int mods) {

    Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
    if (!engine) return;


    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

    }

}




void Engine::exit() {
    //Logger::Log(LogLevel::INFO, std::to_string(layers.size()));
    //Terminate ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(NEngine::window);
    glfwTerminate();
};

void Engine::render() {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // imgui updates here
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            // quit item
            if (ImGui::MenuItem("Close lol")) {
                NEngine::running = false;
            }

            // adds a debug colorfull and random mesh layer to the screen
            if (ImGui::MenuItem("add 200 Mesh Layer")) {
                for (int i = 0; i < 200; i++) {
                    pushLayer(std::make_unique<Mesh>());
                }
            }

            //if (ImGui::MenuItem("add Inspector")) {
            //    pushLayer(std::make_unique<Inspecter>());
            //}


            //if (ImGui::MenuItem("Remove Inspector")) {

            //    for (auto it = layers.begin(); it != layers.end(); ++it) {
            //        if ((*it)->getKind() == LayerKind::Inspector) {
            //            (*it)->onDetach();
            //            layers.erase(it);
            //            break;
            //        }
            //    }
            //}


            ImGui::EndMenu();
        }


        for (auto& layer : layers) {
            if (layer->use)
                layer->onMenuBar();
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Text("FPS: %.1f", io.Framerate);


        ImGui::EndMainMenuBar();
    }

    for (auto& layer : layers) {
        if (layer->use)
            layer->render();
    }






    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers and poll events
    glfwSwapBuffers(NEngine::window);
    glfwPollEvents();
}

void Engine::update() {
    NEngine::user_cam.update();

    for (auto& layer : layers) {
        if (layer->use)
            layer->update();
    }



}




void Engine::pushLayer(std::unique_ptr<Layer> layer) {
    layer->set_id(layers.size());
    layer->onAttach();

    RCTX.layers.push_back(layer.get());

    layers.push_back(std::move(layer));
}



// handles all layer packet types
void Engine::handleLayerPacketQue() {
    
    LRH.handleRequests(RCTX);
}


