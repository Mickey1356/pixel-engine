#define STB_IMAGE_IMPLEMENTATION

#include "sprite.h"

namespace pix2d {
    // pixel constructors
    Pixel::Pixel() {}
    Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b} {}
    Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r{r}, g{g}, b{b}, a{a} {}

    // create an empty sprite
    Sprite::Sprite() {
        width = 0; height = 0;
        sprite_data = nullptr;
    }

    // create a sprite from image
    Sprite::Sprite(const std::string& image) {
        // make sure to flip the image vertically
        stbi_set_flip_vertically_on_load(true);
        int channels;
        unsigned char *data = stbi_load(image.c_str(), &width, &height, &channels, 4);
        if (data) {
            std::cout << channels << std::endl;
            if (sprite_data) delete[] sprite_data;

            sprite_data = new Pixel[width * height];
            for (int i = 0; i < width * height; i++) {
                unsigned char r = data[4 * i];
                unsigned char g = data[4 * i + 1];
                unsigned char b = data[4 * i + 2];
                unsigned char a = data[4 * i + 3];
                sprite_data[i] = Pixel(r, g, b, a);
            }
        } else {
            std::cout << "Failed to read sprite: " << image << std::endl;
        }
        stbi_image_free(data);
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
        if (x >= 0 && y >= 0 && x < width && y < height) {
            // (height - y - 1) to make it such that (0, 0) is the top left
            if (sprite_data) return sprite_data[(height - y - 1) * width + x];
        }
        return Pixel();
    }

    // set pixel at (x, y) to p
    void Sprite::set_pixel(int x, int y, Pixel p) {
        if (x >= 0 && y >= 0 && x < width && y < height) {
            // (height - y - 1) to make it such that (0, 0) is the top left
            if (sprite_data) sprite_data[(height - y - 1) * width + x] = p;
        }
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
