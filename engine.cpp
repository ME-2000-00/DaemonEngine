#include "engine.h"
#include "Layer.h"
#include "LegacyOpenglInitLayer.h"
#include "Mesh.h"
#include "ChunkHandlerLayer.h"
#include "WorldData.h"

#include "NEnNamespace.h"


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

    NEngine::colors[0] = glm::vec3(0, 0.8, 0);
    NEngine::colors[1] = glm::vec3(.6, .4, 0);
    NEngine::colors[2] = glm::vec3(0, 0, 1);

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
    NEngine::user_cam = Camera(glm::vec3(0.0f, WorldData::getNoise(0,0,WorldData::CHUNK_SCALE) * (WorldData::CHUNK_HEIGHT * WorldData::WORLD_HEIGHT) + 2, 0.0f),    // position
        glm::radians(fov),           // FOV in radians
        (float)window_width / window_height,  // aspect ratio
        0.01f, 1000.0f);                 // near/far planes


    // imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // base style



    ImGui_ImplGlfw_InitForOpenGL(NEngine::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // request context init and pointers
	populateRCTX();

    // add on start layers here
    pushLayer(std::make_unique<ChunkHandlerLayer>());
    pushLayer(std::make_unique<LegacyOpenglInitLayer>());



    // Set the clear color (RGBA format, values range from 0.0f to 1.0f)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark cyan
    // enable to capture mouse
    glfwSetInputMode(NEngine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // TODO: re-enable later
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(NEngine::user_cam.m_proj));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(NEngine::user_cam.m_view));


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
        NEngine::destroy = true;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        NEngine::build = true;

    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

    }

}




void Engine::exit() {
    //Terminate ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // cleanup for the layers
    for (auto& layer : layers) {
        if (layer->use)
            layer->onDetach();
    }

    glfwDestroyWindow(NEngine::window);
    glfwTerminate();
};

void Engine::render() {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO  will be handled by modernopengl in future
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(NEngine::user_cam.m_proj));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(NEngine::user_cam.m_view));

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // debug window
    if (debug_mode) {
        ImGui::Begin("Debug Window");
        ImGui::DragFloat(" Cam speed", &cam_speed, 1.0f, 0.1f, 1000.0f);
        ImGui::DragFloat(" dot Scale", &WorldData::dot_size, 0.2f, 0.1f, 100.0f);
        ImGui::DragFloat(" FOV", &fov, 1.0f, 20.0f, 120.0f);

        if (ImGui::Button("Set Cam FOV")) {
            int window_width, window_height;
            glfwGetFramebufferSize(NEngine::window, &window_width, &window_height);
            NEngine::user_cam.setFOV(glm::radians(fov), (float)window_width / window_height, 0.01f, 1000.0f);
        }

        ImGui::DragFloat(" Light X", &NEngine::light.x, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat(" Light Y", &NEngine::light.y, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat(" Light Z", &NEngine::light.z, 0.01f, -1.0f, 1.0f);

        // Loop over your 3 colors
        for (int i = 0; i < 3; i++) {
            // Create a unique label for each color
            std::string label = "Color " + std::to_string(i);

            // ImGui expects a float pointer, glm::vec3 stores x/y/z as floats
            ImGui::ColorEdit3(label.c_str(), &NEngine::colors[i].x);
        }
        ImGui::InputInt("Render Distance", &WorldData::render_distance, 1);


        ImGui::End();
    }


    // imgui updates here
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            // quit item
            if (ImGui::MenuItem("Close lol")) {
                NEngine::running = false;
            }


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



    // Save state, switch to pixel-space ortho for editor
    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();

    //int fb_w, fb_h;
    //glfwGetFramebufferSize(NEngine::window, &fb_w, &fb_h);
    //glOrtho(0, fb_w, 0, fb_h, -1, 1);

    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();

    //// disable depth when drawing 2D editor
    //glDisable(GL_DEPTH_TEST);


    //int window_width, window_height;
    //glfwGetFramebufferSize(NEngine::window, &window_width, &window_height);
    //glColor3f(1.0f, 1.0f, 1.0f);
    //glPointSize(WorldData::dot_size);

    //glBegin(GL_POINTS);
    //glVertex2f(window_width / 2, window_height / 2);
    //glEnd();


    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);


    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers and poll events
    glfwSwapBuffers(NEngine::window);
    glfwPollEvents();
}

void Engine::update() {
    dt = ImGui::GetIO().DeltaTime;

    // Update the camera and movement
    if (glfwGetKey(NEngine::window, GLFW_KEY_W) == GLFW_PRESS)
        NEngine::user_cam.move_forward(cam_speed, dt);

    if (glfwGetKey(NEngine::window, GLFW_KEY_S) == GLFW_PRESS)
        NEngine::user_cam.move_backward(cam_speed, dt);

    if (glfwGetKey(NEngine::window, GLFW_KEY_A) == GLFW_PRESS)
        NEngine::user_cam.move_left(cam_speed, dt);

    if (glfwGetKey(NEngine::window, GLFW_KEY_D) == GLFW_PRESS)
        NEngine::user_cam.move_right(cam_speed, dt);

    if (glfwGetKey(NEngine::window, GLFW_KEY_SPACE) == GLFW_PRESS)
        NEngine::user_cam.move_up(cam_speed, dt);

    if (glfwGetKey(NEngine::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        NEngine::user_cam.move_down(cam_speed, dt);

    if (glfwGetInputMode(NEngine::window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        double xpos, ypos;
        glfwGetCursorPos(NEngine::window, &xpos, &ypos);
        NEngine::user_cam.update_rotation(xpos, ypos);
    }

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

void Engine::populateRCTX() {
    RCTX.cam = &NEngine::user_cam;

    RCTX.layers.clear();

    for (auto& layer : layers) {
        RCTX.layers.push_back(layer.get());
    }

}   
