#include <cstdio>
#include "hub75.hpp"
#include "small_font.hpp"
#include "text.hpp"
#include "util.hpp"

bool choose_glyph(unsigned int &width, unsigned int &height, const unsigned char* &pixels, char digit) {
    switch (digit) {
    case '0':
        width = zero::width;
        height = zero::height;
        pixels = &zero::pixels[0][0];
        break;
    case '1':
        width = one::width;
        height = one::height;
        pixels = &one::pixels[0][0];
        break;
    case '2':
        width = two::width;
        height = two::height;
        pixels = &two::pixels[0][0];
        break;
    case '3':
        width = three::width;
        height = three::height;
        pixels = &three::pixels[0][0];
        break;
    case '4':
        width = four::width;
        height = four::height;
        pixels = &four::pixels[0][0];
        break;
    case '5':
        width = five::width;
        height = five::height;
        pixels = &five::pixels[0][0];
        break;
    case '6':
        width = six::width;
        height = six::height;
        pixels = &six::pixels[0][0];
        break;
    case '7':
        width = seven::width;
        height = seven::height;
        pixels = &seven::pixels[0][0];
        break;
    case '8':
        width = eight::width;
        height = eight::height;
        pixels = &eight::pixels[0][0];
        break;
    case '9':
        width = nine::width;
        height = nine::height;
        pixels = &nine::pixels[0][0];
        break;
    case '.':
        width = decimal::width;
        height = decimal::height;
        pixels = &decimal::pixels[0][0];
        break;
    case ',':
        width = comma::width;
        height = comma::height;
        pixels = &comma::pixels[0][0];
        break;
    case ':':
        width = colon::width;
        height = colon::height;
        pixels = &colon::pixels[0][0];
        break;
    case '/':
        width = slash::width;
        height = slash::height;
        pixels = &slash::pixels[0][0];
        break;
    default:
        width = 0;
        height = 0;
        pixels = nullptr;
        return false;
    }
    return true;
}

bool draw_glyph(Hub75& dst, int x_off, int y_off, unsigned int width, unsigned int height, const unsigned char *pixels, Pixel& foreground, Pixel& background) {

    uint32_t GRID_WIDTH  = (dst.width <= 128) ? dst.width : (dst.width/2);
    uint32_t GRID_HEIGHT = (dst.width <= 128) ? dst.height : (dst.height*2);

    if (x_off >= GRID_WIDTH) {
        return true;
    }
    if ((x_off + (int)width) < 0) {
        return true;
    }

    bool restart = true;
    for (uint y=0; y<std::min((unsigned int)GRID_HEIGHT, height); y++) {
        for (uint x=0; x<width; x++) {
            int pixel_x = (int)x + x_off;
            if (pixel_x >= 0 && pixel_x < GRID_WIDTH) {
                restart = false;
                unsigned char alpha = pixels[(height-1-y)*width+x];
                if (alpha > 0) {
                    std::pair<unsigned int,unsigned int> fb_index = grid_to_framebuffer((unsigned int)pixel_x, y + y_off,
                                                                                        GRID_WIDTH,GRID_HEIGHT,
                                                                                        dst.width,dst.height);

                    dst.set_color(fb_index.first, fb_index.second, alpha_blend(alpha, foreground, background));
                }
            }
        }
    }

    return restart;
}

void draw_string_at(Hub75& dst, uint32_t x, uint32_t y, const char* str, JUSTIFY just) {
    unsigned int str_width = 0;
    unsigned int str_height = 0;
    for (const char* p=str; *p; p++) {
        unsigned int w = 0;
        unsigned int h = 0;
        const unsigned char* pix;
        choose_glyph(w,h,pix,*p);
        str_width += w;
        str_height = std::max(str_height, h);
    }

    int x_off = (just == BOTTOM_RIGHT || just == TOP_RIGHT) ? (-str_width) : 0;
    int y_off = (just == TOP_RIGHT || just == TOP_LEFT) ? (-str_height) : 0;

//    printf("str = %s, offset = %d,%d\n",str,x_off,y_off);

    char prev_char = '!';

    std::map<std::pair<char,char>,int> kerning_table = {
        { std::make_pair('/','/'), -2 },
        { std::make_pair('/','4'), -2 },
        { std::make_pair('7','/'), -2 },
        { std::make_pair('7','4'),  0 },
        { std::make_pair('9','/'), -1 },
    };


    Pixel foreground(255,255,255);
    Pixel background(0,0,0);
    for (const char* p=str; *p; p++) {
        const char curr_char = *p;

        unsigned int w = 0;
        unsigned int h = 0;
        const unsigned char* pix;

        int kern = 2;
        auto it = kerning_table.find(std::make_pair(prev_char,curr_char));
        if (it != kerning_table.end()) {
            kern = it->second;
        } else {
            printf("no kerning entry for %c,%c\n",prev_char,curr_char);
        }
        x_off += kern;

        choose_glyph(w,h,pix,curr_char);
        draw_glyph(dst,x + x_off, y + y_off, w, h, pix, foreground, background);

        x_off += w;


        prev_char = curr_char;
    }
}
