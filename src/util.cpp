#include "hub75.hpp"
#include "util.hpp"

Pixel alpha_blend(unsigned char alpha, const Pixel& foreground, const Pixel& background) {
    if (alpha == 0) {
        return background;
    } else if (alpha == 255) {
        return foreground;
    }

    float a = (float)alpha / 255.f;
    float r = a * (float)( foreground.color      & 0x3ff) / 1023.f + (1.f - a)*(float)( background.color      & 0x3ff) / 1023.f;
    float g = a * (float)((foreground.color>>10) & 0x3ff) / 1023.f + (1.f - a)*(float)((background.color>>10) & 0x3ff) / 1023.f;
    float b = a * (float)((foreground.color>>20) & 0x3ff) / 1023.f + (1.f - a)*(float)((background.color>>20) & 0x3ff) / 1023.f;

    Pixel result = {};
    result.color = (unsigned int)(r * 1023.f) | (unsigned int)(g * 1023.f)<<10 | (unsigned int)(b * 1023.f) << 20;
    return result;
}


std::pair<unsigned int,unsigned int> grid_to_framebuffer(uint32_t x,uint32_t y,
                                     uint32_t grid_width, uint32_t grid_height,
                                     unsigned int fb_width, unsigned int fb_height) {
    unsigned int tx = (unsigned int)x;
    unsigned int ty = (unsigned int)(grid_height - 1 - y);
    if (ty < fb_height) {
        return std::make_pair(tx,ty);
    } else {
        tx = fb_width - 1 - tx;
        ty = (unsigned int)grid_height - 1 - ty;
        return std::make_pair(tx,ty);
    }
}
