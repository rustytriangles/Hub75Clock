#include <cstdint>
#include <utility>

#pragma once

struct Pixel;

Pixel alpha_blend(unsigned char alpha, const Pixel& foreground, const Pixel& background);

std::pair<unsigned int,unsigned int> grid_to_framebuffer(uint32_t x,uint32_t y,
                                                         uint32_t grid_width, uint32_t grid_height,
                                                         unsigned int fb_width, unsigned int fb_height);
