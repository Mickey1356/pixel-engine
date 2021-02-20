#include "engine.h"

namespace pix_eng {
    Engine::Engine() {
        
    }

    Engine::~Engine() {
        if (canvas_sprite) delete canvas_sprite;
        if (font_sprite) delete font_sprite;
    }

    // USER ENGINE FUNCTIONS
    bool Engine::initialise(int canvas_width, int canvas_height, int screen_width, int screen_height, const std::string& eng_name) {
        if (canvas_width < 0 || canvas_height < 0) return false;
        if (screen_width < 0 || screen_height < 0) return false;
        if (is_running) return false;

        this->canvas_width = canvas_width;
        this->canvas_height = canvas_height;
        this->screen_width = screen_width;
        this->screen_height = screen_height;
        if (!eng_name.empty()) engine_name = eng_name;

        Engine::engine_instance = this;

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

    bool Engine::close() {
        if (is_running) is_running = false;
        return is_running;
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

    int Engine::get_screen_width() {
        return screen_width;
    }

    int Engine::get_screen_height() {
        return screen_height;
    }

    void Engine::set_screen_width(int width) {
        screen_width = width;
        glfwSetWindowSize(window, screen_width, screen_height);
    }

    void Engine::set_screen_height(int height) {
        screen_height = height;
        glfwSetWindowSize(window, screen_width, screen_height);
    }

    int Engine::get_canvas_width() {
        return canvas_width;
    }

    int Engine::get_canvas_height() {
        return canvas_height;
    }

    void Engine::set_title(const std::string& title) {
        glfwSetWindowTitle(window, title.c_str());
    }



    // ENGINE WORKINGS
    // setup opengl contexts
    bool Engine::prepare() {
        // opengl setup
        if (!glfwInit()) return false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // map glfw input to the engine's representation
        map_kb[GLFW_KEY_UNKNOWN] = Key::UNKNOWN;
        
        map_kb[GLFW_KEY_SPACE] = Key::SPACE; map_kb[GLFW_KEY_APOSTROPHE] = Key::APOSTROPHE;
        map_kb[GLFW_KEY_COMMA] = Key::COMMA; map_kb[GLFW_KEY_MINUS] = Key::MINUS; map_kb[GLFW_KEY_PERIOD] = Key::PERIOD;
        map_kb[GLFW_KEY_SLASH] = Key::SLASH; map_kb[GLFW_KEY_SEMICOLON] = Key::SEMICOLON; map_kb[GLFW_KEY_EQUAL] = Key::EQUAL;

        map_kb[GLFW_KEY_0] = Key::N0; map_kb[GLFW_KEY_1] = Key::N1; map_kb[GLFW_KEY_2] = Key::N2; map_kb[GLFW_KEY_3] = Key::N3; map_kb[GLFW_KEY_4] = Key::N4;
        map_kb[GLFW_KEY_5] = Key::N5; map_kb[GLFW_KEY_6] = Key::N6; map_kb[GLFW_KEY_7] = Key::N7; map_kb[GLFW_KEY_8] = Key::N8; map_kb[GLFW_KEY_9] = Key::N9;

        map_kb[GLFW_KEY_A] = Key::A; map_kb[GLFW_KEY_B] = Key::B; map_kb[GLFW_KEY_C] = Key::C; map_kb[GLFW_KEY_D] = Key::D; map_kb[GLFW_KEY_E] = Key::E;
        map_kb[GLFW_KEY_F] = Key::F; map_kb[GLFW_KEY_G] = Key::G; map_kb[GLFW_KEY_H] = Key::H; map_kb[GLFW_KEY_I] = Key::I; map_kb[GLFW_KEY_J] = Key::J;
        map_kb[GLFW_KEY_K] = Key::K; map_kb[GLFW_KEY_L] = Key::L; map_kb[GLFW_KEY_M] = Key::M; map_kb[GLFW_KEY_N] = Key::N; map_kb[GLFW_KEY_O] = Key::O;
        map_kb[GLFW_KEY_P] = Key::P; map_kb[GLFW_KEY_Q] = Key::Q; map_kb[GLFW_KEY_R] = Key::R; map_kb[GLFW_KEY_S] = Key::S; map_kb[GLFW_KEY_T] = Key::T;
        map_kb[GLFW_KEY_U] = Key::U; map_kb[GLFW_KEY_V] = Key::V; map_kb[GLFW_KEY_W] = Key::W; map_kb[GLFW_KEY_X] = Key::X; map_kb[GLFW_KEY_Y] = Key::Y;
        map_kb[GLFW_KEY_Z] = Key::Z;

        map_kb[GLFW_KEY_LEFT_BRACKET] = Key::LEFT_BRACKET; map_kb[GLFW_KEY_BACKSLASH] = Key::BACKSLASH;
        map_kb[GLFW_KEY_RIGHT_BRACKET] = Key::RIGHT_BRACKET; map_kb[GLFW_KEY_GRAVE_ACCENT] = Key::TILDE;

        map_kb[GLFW_KEY_ESCAPE] = Key::ESCAPE; map_kb[GLFW_KEY_ENTER] = Key::ENTER; map_kb[GLFW_KEY_TAB] = Key::TAB;
        map_kb[GLFW_KEY_BACKSPACE] = Key::BACKSPACE; map_kb[GLFW_KEY_INSERT] = Key::INSERT; map_kb[GLFW_KEY_DELETE] = Key::DELETE;

        map_kb[GLFW_KEY_RIGHT] = Key::RIGHT; map_kb[GLFW_KEY_LEFT] = Key::LEFT; map_kb[GLFW_KEY_DOWN] = Key::DOWN; map_kb[GLFW_KEY_UP] = Key::UP;
        
        map_kb[GLFW_KEY_F1] = Key::F1; map_kb[GLFW_KEY_F2] = Key::F2; map_kb[GLFW_KEY_F3] = Key::F3;
        map_kb[GLFW_KEY_F4] = Key::F4; map_kb[GLFW_KEY_F5] = Key::F5; map_kb[GLFW_KEY_F6] = Key::F6;
        map_kb[GLFW_KEY_F7] = Key::F7; map_kb[GLFW_KEY_F8] = Key::F8; map_kb[GLFW_KEY_F9] = Key::F9;
        map_kb[GLFW_KEY_F10] = Key::F10; map_kb[GLFW_KEY_F11] = Key::F11; map_kb[GLFW_KEY_F12] = Key::F12;

        map_kb[GLFW_KEY_LEFT_SHIFT] = Key::L_SHIFT; map_kb[GLFW_KEY_LEFT_CONTROL] = Key::L_CTRL; map_kb[GLFW_KEY_LEFT_ALT] = Key::L_ALT;
        map_kb[GLFW_KEY_RIGHT_SHIFT] = Key::R_SHIFT; map_kb[GLFW_KEY_RIGHT_CONTROL] = Key::R_CTRL; map_kb[GLFW_KEY_RIGHT_ALT] = Key::R_ALT;
                        
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
        // set window viewport
        glViewport(0, 0, screen_width, screen_height);

        // create canvas and font sprites
        if (!construct_canvas() || !construct_font()) is_running = false;

        // set some callbacks
        glfwSetFramebufferSizeCallback(window, window_resize_callback); // resizing the window
        glfwSetKeyCallback(window, keyboard_callback); // keyboard input
        glfwSetMouseButtonCallback(window, mouse_button_callback); // mouse button input
        glfwSetCursorPosCallback(window, mouse_pos_callback); // mouse position

        // enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        // we are now comparing the new state with the old state
        // keyboard input
        for (int i = 0; i < NUM_KEYS; i++) {
            // reset the keyboard states that are only active for a single frame
            keyboard_cur[i].pressed = false;
            keyboard_cur[i].released = false;

            // if the new state is different from the old state
            if (keyboard_new[i] != keyboard_old[i]) {
                // if the new state is true
                if (keyboard_new[i]) {
                    // then pressed and held will be true
                    keyboard_cur[i].pressed = true;
                    keyboard_cur[i].held = true;
                } else {
                    keyboard_cur[i].released = true;
                    keyboard_cur[i].held = false;
                }
            }
            // the new state is now the old state
            keyboard_old[i] = keyboard_new[i];
        }

        // mouse input
        for (int i = 0; i < NUM_MOUSE; i++) {
            // reset the keyboard states that are only active for a single frame
            mbtn_cur[i].pressed = false;
            mbtn_cur[i].released = false;

            // if the new state is different from the old state
            if (mbtn_new[i] != mbtn_old[i]) {
                // if the new state is true
                if (mbtn_new[i]) {
                    // then pressed and held will be true
                    mbtn_cur[i].pressed = true;
                    mbtn_cur[i].held = true;
                } else {
                    mbtn_cur[i].released = true;
                    mbtn_cur[i].held = false;
                }
            }
            // the new state is now the old state
            mbtn_old[i] = mbtn_new[i];
        }


        // do user update
        if (!update(delta_time)) {
            is_running = false;
            return;
        }

        // background fill
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
        canvas_sprite = new Sprite(canvas_width, canvas_height);

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
        
        // set filter to nearest neighbour (for the pixelly look)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

        // build the shaders
        // shaders are stored in res/defaults/canvas.[frag|vert]
        canvas_shader = Shader("res/defaults/canvas.vert", "res/defaults/canvas.frag");

        return true;
    }

    // compiles and loads the default font
    bool Engine::construct_font() {
        // default font is the atari character set limited to the printable ascii characters
        // font data (sprite data) is stored in res/defaults/default_font.png 
        font_sprite = new Sprite("res/defaults/font.png");
        if (font_sprite) return true;
        else return false;
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

        // get original pixel colour
        Pixel op = canvas_sprite->get_pixel(x, y);

        // convert to doubles (0 - 1)
        double or = op.r / 255.0, og = op.g / 255.0, ob = op.b / 255.0, oa = op.a / 255.0;
        // same with the target pixel
        double tr = p.r / 255.0, tg = p.g / 255.0, tb = p.b / 255.0, ta = p.a / 255.0;

        // premultiply the alphas
        or *= oa; og *= oa; ob *= oa;
        tr *= ta; tg *= ta; tb *= ta;

        // get new alpha
        double na = ta + (1 - ta) * oa;

        // over operator
        double nr, ng, nb;
        nr = tr + (1 - ta) * or;
        ng = tg + (1 - ta) * og;
        nb = tb + (1 - ta) * ob;

        // divide out the alpha (if alpha is non-zero)
        if (na > 0) {
            nr /= na; ng /= na; nb /= na;
        }

        // convert back to ints
        uint8_t ir, ig, ib, ia;
        ir = (uint8_t) floor(nr * 255.0);
        ig = (uint8_t) floor(ng * 255.0);
        ib = (uint8_t) floor(nb * 255.0);
        ia = (uint8_t) floor(na * 255.0);

        Pixel np = Pixel(ir, ig, ib, ia);

        canvas_sprite->set_pixel(x, y, np);
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

        // draw the inside (offset by 1 to include the edges)
        for (int x = x1 + 1; x < x1 + w - 1; x++) {
            for (int y = y1 + 1; y < y1 + h - 1; y++) {
                point(x, y, f);
            }
        }

        // draw the edges
        for (int x = x1; x < x1 + w; x++) {
            point(x, y1, s);
            point(x, y1 + h - 1, s);
        }
        for (int y = y1 + 1; y < y1 + h - 1; y++) {
            point(x1, y, s);
            point(x1 + w - 1, y, s);
        }
    }

    // draws a triangle using points (x1, y1), (x2, y2), (x3, y3) with stroke s and fill f
    void Engine::triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s) {

    }

    void Engine::triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s, Pixel f) {

    }

    // draws a sprite at (x, y)
    void Engine::draw_sprite(int x, int y, Sprite* sprite) {
        for (int i = 0; i < sprite->get_width(); i++) {
            for (int j = 0; j < sprite->get_height(); j++) {
                point(x + i, y + j, sprite->get_pixel(i, j));
            }
        }
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


    // CALLBACK/INPUT FUNCTIONS
    // get the current status of a keyboard key
    Button Engine::get_key(Key k) {
        return keyboard_cur[k];
    }

    // get the current status of a mouse button (LMB: 0, RMB: 1, MID: 2)
    Button Engine::get_mouse_btn(int button) {
        if (button >= 0 && button < NUM_MOUSE) return mbtn_cur[button];
        else return Button();
    }

    // get absolute x position of mouse
    double Engine::get_mouseX_abs() {
        return mouseX;
    }

    // get absolution y position of mouse
    double Engine::get_mouseY_abs() {
        return mouseY;
    }

    // get relative x position of mouse (0 is left, 1 is right)
    double Engine::get_mouseX_rel() {
        return (double) mouseX / screen_width;
    }

    // get relative y position of mouse (0 is up, 1 is down)
    double Engine::get_mouseY_rel() {
        return (double) mouseY / screen_height;
    }


    void Engine::window_resize_callback(GLFWwindow* window, int width, int height) {
        int w, h;
        engine_instance->screen_width = width;
        engine_instance->screen_height = height;
        glfwGetFramebufferSize(window, &w, &h);
	    glViewport(0, 0, w, h);
    }

    void Engine::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (engine_instance->map_kb.count(key) != 0) {
            // use mp_kb to turn key into the engine's representation
            int eng_rep = engine_instance->map_kb[key];

            // set the appropriate flag for the engine's representation
            if (action == GLFW_PRESS) {
                engine_instance->keyboard_new[eng_rep] = true;
            } else if (action == GLFW_RELEASE) {
                engine_instance->keyboard_new[eng_rep] = false;
            }
        }
    }

    void Engine::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        if (button >= 0 && button < NUM_MOUSE) {
            if (action == GLFW_PRESS) {
                engine_instance->mbtn_new[button] = true;
            } else if (action == GLFW_RELEASE) {
                engine_instance->mbtn_new[button] = false;
            }
        }
    }

    void Engine::mouse_pos_callback(GLFWwindow* window, double x, double y) {
        engine_instance->mouseX = x;
        engine_instance->mouseY = y;
    }

    Engine* Engine::engine_instance = nullptr;
    std::atomic<bool> Engine::is_running { false };
}