#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <atomic>
#include <iostream>
#include <thread>
#include <string>

#include "sprite.h"
#include "shader.h"

namespace pix_eng {

    class Engine {
    public: // constructors
        Engine();
        ~Engine();

    public: // user engine functions
        // change the default screen sizes, call before start
        bool initialise(int width, int height, const std::string& eng_name);
        // user called: call prepare, start thread, wait for thread to finish
        bool start();
        // called when engine initialises
        virtual bool create();
        // called every frame
        virtual bool update(double delta_time);
        // called when engine stops
        virtual bool destroy();

    public: // engine workings
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
        
    public: // additional variables
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

        // canvas quad
        // static float canvas_quad[];
        // static int canvas_indices[];
    };
}

#endif