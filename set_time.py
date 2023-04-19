import machine
import utime
import time
import ustruct
import sys

###############################################################################
# Constants

# I2C address
RV1805_ADDR = 0x69

# Registers
RV1805_ID0 = 0x28
RV1805_PART_NUMBER_UPPER = 0x18
RV1805_CONF_KEY = 0x1F
RV1805_CTRL1 = 0x10
RV1805_TRICKLE_CHRG = 0x20
RV1805_IOBATMODE= 0x27
RV1805_OUT_CTRL = 0x30
RV1805_HUNDREDTHS = 0x00
RV1805_SECONDS = 0x01
RV1805_MINUTES = 0x02
RV1805_HOURS = 0x03
RV1805_DATE = 0x04
RV1805_MONTHS = 0x05
RV1805_YEARS = 0x06
RV1805_WEEKDAYS = 0x07


RV1805_CONF_WRT = 0x9D #value written to Configuration Key to enable write of trickle charge, BREF, CAPRC, IO Batmo\
RV1805_CAP_RC = 0x26

REG_DEVID = 0x00
REG_POWER_CTL = 0x2D
REG_DATAX0 = 0x32

###############################################################################
# Settings

# Initialize I2C with pins
i2c = machine.I2C(0,
                  scl=machine.Pin(21),
                  sda=machine.Pin(20),
                  freq=400000)



###############################################################################
# Functions
def bcd2dec(bcd):
    return (((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f))

def dec2bcd(dec):
    tens, units = divmod(dec, 10)
    return (tens << 4) + units

def reg_write(i2c, addr, reg, data):
    """
    Write bytes to the specified register.
    """
    
    # Construct message
    msg = bytearray()
    msg.append(data)
    
    # Write out message to register
    i2c.writeto_mem(addr, reg, msg)
    
def reg_read(i2c, addr, reg, nbytes=1):
    """
    Read byte(s) from specified register. If nbytes > 1, read from consecutive
    registers.
    """
    
    # Check to make sure caller is asking for 1 or more bytes
    if nbytes < 1:
        return bytearray()
    
    # Request data from specified register(s) over I2C
    data = i2c.readfrom_mem(addr, reg, nbytes)
    
    return data

###############################################################################
# Main

# Read device ID to make sure that we can communicate with the ADXL343
data = reg_read(i2c, RV1805_ADDR, RV1805_ID0)
if (data != bytearray((RV1805_PART_NUMBER_UPPER,))):
    print("ERROR: Could not communicate with RV1805")
    sys.exit()
    
reg_write(i2c, RV1805_ADDR, RV1805_CONF_KEY, RV1805_CONF_WRT) #Enable write access to the CAPRC Register (26h)                                     
reg_write(i2c, RV1805_ADDR, RV1805_CAP_RC, 0xA0); #Enable Cap_RC pin                                                                                

# enable tricklecharge
reg_write(i2c, RV1805_ADDR, RV1805_CONF_KEY, RV1805_CONF_WRT) #Enable write access to the CAPRC Register (26h)                                     
value = 0b10100101
reg_write(i2c, RV1805_ADDR, RV1805_TRICKLE_CHRG, value)

# enable lowpower
reg_write(i2c, RV1805_ADDR, RV1805_CONF_KEY, RV1805_CONF_WRT) #Enable write access to the CAPRC Register (26h)                                     
reg_write(i2c, RV1805_ADDR, RV1805_IOBATMODE, 0)
reg_write(i2c, RV1805_ADDR, RV1805_CONF_KEY, RV1805_CONF_WRT) #Enable write access to the CAPRC Register (26h)                                     
reg_write(i2c, RV1805_ADDR, RV1805_OUT_CTRL, 0x30)

data = reg_read(i2c, RV1805_ADDR, RV1805_CTRL1)
#print("data = " + str(data[0]))
if (data[0] & 1<<2) != 0:
    print("ARST is true")    
if (data[0] & 1<<3) != 0:
    print("RSTP is true")
if (data[0] & 1<<5) != 0:
    print("PSWB is true")
if (data[0] & 1<<6) != 0:
    print("12_24 is true")
if (data[0] & 1<<7) != 0:
    print("STOP is true")

value = data[0] | 1<<2
reg_write(i2c, RV1805_ADDR, RV1805_CTRL1, value)

# Read Power Control register
data = reg_read(i2c, RV1805_ADDR, REG_POWER_CTL)
#print(str(data[0]))

t = time.localtime()

data = dec2bcd(t[0] - 2000)
reg_write(i2c, RV1805_ADDR, RV1805_YEARS, data)

data = dec2bcd(t[1])
reg_write(i2c, RV1805_ADDR, RV1805_MONTHS, data)

data = dec2bcd(t[2])
reg_write(i2c, RV1805_ADDR, RV1805_DATE, data)

data = dec2bcd(t[3])
reg_write(i2c, RV1805_ADDR, RV1805_HOURS, data)

data = dec2bcd(t[4])
reg_write(i2c, RV1805_ADDR, RV1805_MINUTES, data)

data = dec2bcd(t[5])
reg_write(i2c, RV1805_ADDR, RV1805_SECONDS, data)
