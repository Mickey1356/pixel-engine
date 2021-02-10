#include "engine.h"

namespace pix_eng {
    Engine::Engine() {
        
    }

    Engine::~Engine() {

    }

    // USER ENGINE FUNCTIONS
    bool Engine::initialise(int width, int height, const std::string& eng_name) {
        if (width < 0 || height < 0) return false;
        if (is_running) return false;

        screen_width = width;
        screen_height = height;
        engine_name = eng_name;
        return true;
    }

    // user called: calls prepare, starts thread, wait for thread to finish, cleanup
    bool Engine::start() {
        if (!prepare()) return false;

        is_running = true;
        std::thread t = std::thread(&Engine::engine_thread, this);

        t.join();

        if (!clean_up()) return false;

        return true;
    }

    // called when engine initialises
    bool Engine::create() {
        std::cout << "Failed to override Engine::create()" << std::endl;
        return false;
    }

    // called every frame
    bool Engine::update(double delta_time) {
        std::cout << "Failed to override Engine::update()" << std::endl;
        return false;
    }

    // called when engine stops
    bool Engine::destroy() {
        return true;
    }

    // ENGINE WORKINGS
    // setup opengl contexts
    bool Engine::prepare() {
        // opengl setup
        if (!glfwInit()) return false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        return true;
    }

    // threaded function, first calls create, then continuously updates engine, then calls destroy
    void Engine::engine_thread() {
        // set up window, as well as load glad
        thread_prepare();

        // run user create function
        if (!create()) is_running = false;

        // update loop
        while (is_running) {
            while (is_running) engine_update();

            if (!destroy()) is_running = true;
        }

        std::cout << is_running << " thread" << std::endl;
    }

    // clean up
    bool Engine::clean_up() {
        glfwTerminate();
        return true;
    }

    // thread-specific preparation
    bool Engine::thread_prepare() {
        // create window
        window = glfwCreateWindow(screen_width, screen_height, engine_name.c_str(), NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialise GLAD" << std::endl;
            return false;
        }

        // show the window
        glViewport(0, 0, screen_width, screen_height);

        // create canvas and font sprites
        if (!construct_canvas() || !construct_font()) is_running = false;

        // reset the times
        time_1 = glfwGetTime() * 1000.0;
        time_2 = glfwGetTime() * 1000.0;
        return true;
    }

    // thread calls this, checks for input, user update, draw graphics
    void Engine::engine_update() {
        // check if user tries to close window
        if (glfwWindowShouldClose(window)) {
            is_running = false;
            return;
        }
        
        // calculate delta_time
        time_2 = glfwGetTime() * 1000.0;
        double delta_time = time_2 - time_1;
        time_1 = time_2;

        // check input events


        // do user update
        if (!update(delta_time)) {
            is_running = false;
            return;
        }

        // background fill
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw canvas to screen
        // bind texture buffer (glBindTexture)
        // update texture buffer (glTexImage2D)
        // create and draw quad to screen (bind vertex buffer, buffer vertex info, glDrawArrays)


        // swap buffers and display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // initialises canvas sprite
    bool Engine::construct_canvas() {
        return true;
    }

    // compiles and loads the default font
    bool Engine::construct_font() {
        return true;
    }

    // DRAWING FUNCTIONS
    // draws a point at (x, y)
    void Engine::point(int x, int y, Pixel p) {
        if (!canvas_sprite) return;
        canvas_sprite->set_pixel(x, y, p);
    }

    // draws a line from (x1, y1) to (x2, y2)
    void Engine::line(int x1, int y1, int x2, int y2, Pixel p) {

    }

    // draws a circle at (x1, y1) with radius r with stroke s, and fill f
    void Engine::circle(int x1, int y1, int r, Pixel s) {

    }

    void Engine::circle(int x1, int y1, int r, Pixel s, Pixel f) {

    }

    // draws a rect at (x1, y1) to (x1 + w, y1 + h) with stroke s and fill f
    void Engine::rect(int x1, int y1, int w, int h, Pixel s) {

    }

    void Engine::rect(int x1, int y1, int w, int h, Pixel s, Pixel f) {

    }

    // draws a triangle using points (x1, y1), (x2, y2), (x3, y3) with stroke s and fill f
    void Engine::triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s) {

    }

    void Engine::triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s, Pixel f) {

    }

    // draws a sprite at (x, y)
    void Engine::sprite(int x, int y, const Sprite* sprite) {

    }

    // draws text at (x, y) with fill c
    void Engine::text(int x, int y, const std::string& text, Pixel c) {

    }

    // GLFWwindow* Engine::window = nullptr;
    std::atomic<bool> Engine::is_running { false };
}