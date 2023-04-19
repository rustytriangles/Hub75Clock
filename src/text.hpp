#include <cstdint>
#include <map>

#pragma once

enum JUSTIFY {
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    TOP_LEFT,
    TOP_RIGHT
};

void draw_string_at(Hub75& dst, uint32_t x, uint32_t y, const char* str, JUSTIFY just);
