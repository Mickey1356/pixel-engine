#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <iostream>

namespace pix_eng {

    struct Pixel {
        uint8_t r = 0, g = 0, b = 0, a = 255;
        Pixel();
        Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    };

    class Sprite {
    public: // constructors
        // create an empty sprite
        Sprite();
        // create a sprite from image
        Sprite(const std::string& image);
        // create an empty sprite with width w and height h
        Sprite(int w, int h);
        ~Sprite();

    public: // sprite data
        // get width and height
        int get_width();
        int get_height();
        // get pixel at (x, y)
        Pixel get_pixel(int x, int y);
        // set pixel at (x, y) to p
        void set_pixel(int x, int y, Pixel p);
        // get a copy of this sprite
        Sprite* duplicate();
        // get the array representing this sprite
        Pixel* get_data();

    private:
        int width = 0, height = 0;
        Pixel* sprite_data = nullptr;
        
        // texture ids
        unsigned int texture;
    };

}

#endif