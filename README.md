# pico

## hardward

pico

## environment setup

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib

# use rasp/openocd
sudo apt install libusb-1.0-0-dev libhidapi-dev

../configure --prefix=$(pwd)/output --enable-cmsis-dap --enable-cmsis-dap-v2

openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "program blink/blink.elf"
```

### sdk

https://github.com/raspberrypi/pico-sdk
https://github.com/raspberrypi/pico-sdk/archive/refs/tags/1.5.1.tar.gz
https://github.com/raspberrypi/pico-sdk/releases/download/1.5.1/sdk-1.5.1-html-doc.zip
https://github.com/raspberrypi/pico-examples.git
https://github.com/raspberrypi/openocd.git

### 

