#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <atomic>
#include <iostream>
#include <thread>
#include <string>
#include <map>

#include "sprite.h"
#include "shader.h"

namespace pix_eng {

    // we create space for 128 keys (even though we are only using 77)
    const int NUM_KEYS = 128;
    // glfw supports 8 mouse buttons
    const int NUM_MOUSE = 8;

    struct Button {
        bool pressed = false; // true on the frame when the button is pressed
        bool released = false; // true on the frame when the button is released
        bool held = false; // true on all frames from when button is pressed to when it is released
    };

    // list valid keys (we are going to use a normal US keyboard for now)
    enum Key {
        UNKNOWN,
        SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH, SEMICOLON, EQUAL,
        N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, // number keys
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, TILDE,
        ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE,
        RIGHT, LEFT, DOWN, UP,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        L_SHIFT, L_CTRL, L_ALT, R_SHIFT, R_CTRL, R_ALT,
    };

    class Engine {
    public: // constructors
        Engine();
        ~Engine();

    public: // user engine functions
        // change the default screen sizes, call before start
        bool initialise(int canvas_width=640, int canvas_height=480, int screen_width=640, int screen_height=480, const std::string& eng_name="");
        // user called: call prepare, start thread, wait for thread to finish
        bool start();
        // user called: used to quit the engine
        bool close();
        // called when engine initialises
        virtual bool create();
        // called every frame
        virtual bool update(double delta_time);
        // called when engine stops
        virtual bool destroy();

        // getters and setters for some engine variables
        int get_screen_width(); int get_screen_height();
        void set_screen_width(int width); void set_screen_height(int height);
        int get_canvas_width(); int get_canvas_height();
        void set_title(const std::string& title);


    private: // engine workings
        // setup opengl contexts
        bool prepare();
        // threaded function, first calls create, then continuously updates engine, then calls destroy
        void engine_thread();
        // clean up
        bool clean_up();

        // thread-specific preparation
        bool thread_prepare();
        // thread calls this, checks for input, user update, draw graphics
        void engine_update();

        // initialises canvas sprite
        bool construct_canvas();
        // compiles and loads the default font
        bool construct_font();

        // draw canvas to screen
        bool draw_canvas();        

    public: // drawing functions
        // draws a point at (x, y)
        void point(int x, int y, Pixel p);
        // draws a line from (x1, y1) to (x2, y2)
        void line(int x1, int y1, int x2, int y2, Pixel p);
        // draws a circle at (x1, y1) with radius r with stroke s, and fill f
        void circle(int x1, int y1, int r, Pixel s);
        void circle(int x1, int y1, int r, Pixel s, Pixel f);
        // draws a rect at (x1, y1) to (x1 + w, y1 + h) with stroke s and fill f
        void rect(int x1, int y1, int w, int h, Pixel s);
        void rect(int x1, int y1, int w, int h, Pixel s, Pixel f);
        // draws a triangle using points (x1, y1), (x2, y2), (x3, y3) with stroke s and fill f
        void triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s);
        void triangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel s, Pixel f);
        // draws a sprite at (x, y)
        void sprite(int x, int y, const Sprite* sprite);
        // draws text at (x, y) with fill c
        void text(int x, int y, const std::string& text, Pixel c);
        // clears the screen with fill c
        void clear(Pixel c);

    public: // callback and input functions
        Button get_key(Key k);
        Button get_mouse_btn(int button);
        double get_mouseX_abs();
        double get_mouseY_abs();
        double get_mouseX_rel();
        double get_mouseY_rel();

        // for resizing
        static void window_resize_callback(GLFWwindow* window, int width, int height);
        static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void mouse_pos_callback(GLFWwindow* window, double x, double y);
        
    private: // additional variables
        int canvas_width = 640, canvas_height = 480;
        int screen_width = 640, screen_height = 480;
        std::string engine_name = "Pixel Engine";

        double time_1, time_2;

        Sprite* font_sprite = nullptr;
        Sprite* canvas_sprite = nullptr;

        GLFWwindow* window = nullptr;
        static std::atomic<bool> is_running;

        // graphic buffers and stuff
        unsigned int canvas_texture;
        unsigned int canvas_vbo, canvas_vao, canvas_ebo;
        Shader canvas_shader;

        // input buttons
        // maintain a static variable that keeps track of the engine instance (so static callbacks can modify instance variables)
        static Engine* engine_instance;
        std::map<int, int> map_kb;

        Button keyboard_cur[NUM_KEYS] = { 0 };
        bool keyboard_old[NUM_KEYS] = { 0 };
        bool keyboard_new[NUM_KEYS] = { 0 };

        Button mbtn_cur[NUM_MOUSE] = { 0 };
        bool mbtn_old[NUM_MOUSE] = { 0 };
        bool mbtn_new[NUM_MOUSE] = { 0 };

        double mouseX = 0, mouseY = 0;
    };
}

#endif