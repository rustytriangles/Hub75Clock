#include <algorithm>
#include <cstdio>

#include "hardware/i2c.h"
#include "hardware/vreg.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include "hub75.hpp"
#include "person_sensor.hpp"
#include "rv1805.hpp"
#include "text.hpp"
#include "util.hpp"

const uint32_t FB_WIDTH = 128;
const uint8_t FB_HEIGHT = 64;

const uint32_t GRID_WIDTH  = 128;
const uint32_t GRID_HEIGHT = 64;

Hub75 hub75(FB_WIDTH, FB_HEIGHT, nullptr, PANEL_GENERIC, true);

//#define USE_PERSON_SENSOR 1
#define USE_RV1805 1

const int32_t I2C_BAUD_RATE = (400 * 1000);
const uint I75_SDA_PIN = 20; // was PICO_DEFAULT_I2C_SDA_PIN
const uint I75_SCL_PIN = 21; // was PICO_DEFAULT_I2C_SCL_PIN

void __isr dma_complete() {
    hub75.dma_complete();
}

int main() {

    stdio_init_all();

    i2c_init(i2c_default, I2C_BAUD_RATE);
    gpio_set_function(I75_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I75_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I75_SDA_PIN);
    gpio_pull_up(I75_SCL_PIN);
    // Make the I2C pins available to picotool.
    bi_decl(bi_2pins_with_func(I75_SDA_PIN,
                               I75_SCL_PIN, GPIO_FUNC_I2C));

#ifdef USE_PERSON_SENSOR
    person_sensor_init();
#endif

    hub75.start(dma_complete);

    // 1.3v allows overclock to ~280000-300000 but YMMV. Faster clock = faster screen update rate!
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    sleep_ms(100);

    // 200MHz is roughly about the lower limit for driving a 64x64 display smoothly.
    // Just don't look at it out of the corner of your eye.
    set_sys_clock_khz(250000, false);

    Pixel background(0.f, 0.f, 0.f);

    while (true) {
        hub75.clear();

        char buff[24];

#ifdef USE_RV1805
        TimeResults time_results;
        read_time(time_results);


        sprintf(buff,"%d:%02d:%02d",time_results.hours,time_results.minutes,time_results.seconds);
        draw_string_at(hub75,8,GRID_HEIGHT/2 - 4,buff,TOP_LEFT);

        sprintf(buff,"%d/%d/%d",time_results.day,time_results.month,time_results.year);
        draw_string_at(hub75,8,GRID_HEIGHT/2 + 4,buff,BOTTOM_LEFT);
#endif

#ifdef USE_PERSON_SENSOR
        person_sensor_results_t person_results = {};

        sprintf(buff,"%d",person_results.num_faces);
        draw_string_at(hub75,8,GRID_HEIGHT/2 - 32,buff,TOP_LEFT);
#endif
        hub75.flip(true); // Flip and clear to the background colour

        sleep_ms(100);
    }
}
