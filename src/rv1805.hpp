#include <cstdint>

#pragma once

#define RV1805_ADDR						(uint8_t)0x69

#define RV1805_PART_NUMBER_UPPER 0x18

//Register names:
#define RV1805_HUNDREDTHS               0x00
#define RV1805_SECONDS      			0x01
#define RV1805_MINUTES      			0x02
#define RV1805_HOURS        			0x03
#define RV1805_DATE         			0x04
#define RV1805_MONTHS        			0x05
#define RV1805_YEARS        			0x06
#define RV1805_WEEKDAYS      			0x07
#define RV1805_HUNDREDTHS_ALM           0x08
#define RV1805_SECONDS_ALM    			0x09
#define RV1805_MINUTES_ALM     			0x0A
#define RV1805_HOURS_ALM       			0x0B
#define RV1805_DATE_ALM        			0x0C
#define RV1805_MONTHS_ALM      			0x0D
#define RV1805_WEEKDAYS_ALM    			0x0E
#define RV1805_STATUS					0x0F
#define RV1805_CTRL1					0x10
#define RV1805_CTRL2					0x11
#define RV1805_INT_MASK					0x12
#define RV1805_SQW						0x13
#define RV1805_CAL_XT					0x14
#define RV1805_CAL_RC_UP				0x15
#define RV1805_CAL_RC_LO				0x16
#define RV1805_SLP_CTRL					0x17
#define RV1805_CTDWN_TMR_CTRL			0x18
#define RV1805_CTDWN_TMR				0x19
#define RV1805_TMR_INITIAL				0x1A
#define RV1805_WATCHDOG_TMR				0x1B
#define RV1805_OSC_CTRL					0x1C
#define RV1805_OSC_STATUS				0x1D
#define RV1805_CONF_KEY					0x1F
#define RV1805_TRICKLE_CHRG				0x20
#define RV1805_BREF_CTRL				0x21
#define RV1805_CAP_RC					0x26
#define RV1805_IOBATMODE				0x27
#define RV1805_ID0						0x28
#define RV1805_ANLG_STAT				0x2F
#define RV1805_OUT_CTRL					0x30
#define RV1805_RAM_EXT					0x3F


void rv1805_init();

struct TimeResults {
    int hours;
    int minutes;
    int seconds;
    int day;
    int month;
    int year;
};

void read_time(TimeResults& time_results);
