#include "engine.h"

namespace pix_eng {
    Engine::Engine() {
        
    }

    Engine::~Engine() {
        if (canvas_sprite) delete canvas_sprite;
        if (font_sprite) delete font_sprite;
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
        if (!thread_prepare()) {
            is_running = false;
            return;
        }

        // run user create function
        if (!create()) {
            is_running = false;
            return;
        }

        // update loop
        while (is_running) {
            while (is_running) engine_update();

            if (!destroy()) is_running = true;
        }
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
        return is_running;
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
        if (!draw_canvas()) {
            is_running = false;
            return;
        }

        // swap buffers and display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // initialises canvas sprite
    bool Engine::construct_canvas() {
        canvas_sprite = new Sprite(screen_width, screen_height);

        // define the canvas quad and indices
        float canvas_quad[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
        };

        int canvas_indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        // generate the texture
        glGenTextures(1, &canvas_texture);
        glBindTexture(GL_TEXTURE_2D, canvas_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // generate the buffer objects
        glGenVertexArrays(1, &canvas_vao);
        glGenBuffers(1, &canvas_vbo);
        glGenBuffers(1, &canvas_ebo);

        glBindVertexArray(canvas_vao);
        glBindBuffer(GL_ARRAY_BUFFER, canvas_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(canvas_quad), canvas_quad, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvas_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(canvas_indices), canvas_indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // build the shader
        canvas_shader = Shader("res/shaders/canvas.vert", "res/shaders/canvas.frag");

        return true;
    }

    // compiles and loads the default font
    bool Engine::construct_font() {
        
        return true;
    }

    // draw canvas to screen
    bool Engine::draw_canvas() {
        if (!canvas_sprite) return false;

        if (canvas_sprite->get_data()) {
            // bind texture buffer (glBindTexture)
            glActiveTexture(GL_TEXTURE0); // optional, but set it just in case
            glBindTexture(GL_TEXTURE_2D, canvas_texture);
            // update texture buffer (glTexImage2D)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas_sprite->get_width(), canvas_sprite->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*) canvas_sprite->get_data());
            // create and draw quad to screen (bind vertex buffer, buffer vertex info, glDrawArrays)
            glBindVertexArray(canvas_vao);
            canvas_shader.use();
            canvas_shader.set_int("cTex", 0); // optional, but set it just in case
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
            glBindVertexArray(0);
        } else return false;

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
        for (int x = x1; x < x1 + w; x++) {
            for (int y = y1; y < y1 + h; y++) {
                point(x, y, f);
            }
        }
        for (int x = x1; x < x1 + w; x++) {
            point(x, y1, s);
            point(x, y1 + h, s);
        }
        for (int y = y1; y < y1 + h; y++) {
            point(x1, y, s);
            point(x1 + w, y, s);
        }
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

    // clears the screen with fill c
    void Engine::clear(Pixel c) {
        int pixels = canvas_sprite->get_width() * canvas_sprite->get_height();
        Pixel* m = canvas_sprite->get_data();
        for (int i = 0; i < pixels; i++) m[i] = c;
    }

    std::atomic<bool> Engine::is_running { false };
}