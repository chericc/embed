# pico

## hardward

ESP-01

pico

## environment setup

### cross compiler

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

-arm-none-eabi

### sdk

windows:

https://github.com/raspberrypi/pico-sdk
https://github.com/raspberrypi/pico-setup-windows

https://github.com/raspberrypi/pico-examples/tree/master

### esp32

ESP8266 - ESP-01

sdk:
https://github.com/esp8266/Arduino/tree/master

docs:
https://arduino-esp8266.readthedocs.io/en/latest/



### env setup

cmake 
gcc
python
pictool
pico-sdk-tools
openocd
git
ninja

## problems

1. 如果把pico的uart1（5、6）作为uart通信口，则默认电平会引起esp-01启动模式异常。
2. 如果esp8266晶振频率调整为40MHz，则串口频率不是115200（设置无效）。