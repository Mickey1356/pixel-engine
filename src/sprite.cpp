#include "sprite.h"

namespace pix_eng {
    // pixel constructors
    Pixel::Pixel() {
        r = 0; g = 0; b = 0; a = 255;
    }
    Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r{r}, g{g}, b{b}, a{a} {}

    // create an empty sprite
    Sprite::Sprite() {
        sprite_data = nullptr;
        width = 0; height = 0;
    }

    // create a sprite from image
    Sprite::Sprite(const std::string& image) {

    }

    // create an empty sprite with width w and height h
    Sprite::Sprite(int w, int h) {
        width = w;
        height = h;
        if (sprite_data) delete[] sprite_data;

        sprite_data = new Pixel[width * height];
        for (int i = 0; i < width * height; i++) {
            sprite_data[i] = Pixel();
        }
    }

    Sprite::~Sprite() {
        if (sprite_data) delete[] sprite_data;
    }

    // SPRITE DATA
    // get width and height
    int Sprite::get_width() {
        return width;
    }

    int Sprite::get_height() {
        return height;
    }

    // get pixel at (x, y)
    Pixel Sprite::get_pixel(int x, int y) {
        if (sprite_data) return sprite_data[y * width + x];
        else return Pixel();
    }

    // set pixel at (x, y) to p
    void Sprite::set_pixel(int x, int y, Pixel p) {

    }

    // get a copy of this sprite
    Sprite* Sprite::duplicate() {
        return nullptr;
    }

    // get the array representing this sprite
    Pixel* Sprite::get_data() {
        return sprite_data;
    }

}
