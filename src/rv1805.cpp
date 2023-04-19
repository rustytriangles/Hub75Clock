#include <cstdio>
#include "rv1805.hpp"
#include "hardware/i2c.h"

uint8_t sensorPartNumber = 0;

#define RV1805_ID0						0x28

uint8_t read_register(uint8_t reg) {

    i2c_write_blocking(i2c_default, RV1805_ADDR, &reg, 1, true);
    uint8_t data = 0;
    int num_bytes_read = i2c_read_blocking(i2c_default, RV1805_ADDR, &data, 1, false);  // false - finished with bus
    if (num_bytes_read != 1) {
        printf("Error: Could not read from register %x\n",(int)reg);
    }
    return data;
}

int bcd2dec(int bcd) {

    return (((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f));
}

void rv1805_init() {

//    uint8_t reg = RV1805_ID0;
//    i2c_write_blocking(i2c_default, RV1805_ADDR, &reg, 1, true);
//    uint8_t data = 0;
//    i2c_read_blocking(i2c_default, RV1805_ADDR, &data, 1, false);  // false - finished with bus

    if (read_register(RV1805_ID0) != RV1805_PART_NUMBER_UPPER) {
        printf("ERROR: Could not communicate with RV1805\n");
    }
}

void read_time(TimeResults& results) {

    results.hours = bcd2dec(read_register(RV1805_HOURS));
    results.minutes = bcd2dec(read_register(RV1805_MINUTES));
    results.seconds = bcd2dec(read_register(RV1805_SECONDS));
    printf("%d:%d:%d\n",results.hours,results.minutes,results.seconds);

    results.day = bcd2dec(read_register(RV1805_DATE));
    results. month = bcd2dec(read_register(RV1805_MONTHS));
    results.year = bcd2dec(read_register(RV1805_YEARS)) + 2000;
    printf("%d/%d/%d\n",results.day,results.month,results.year);
}
