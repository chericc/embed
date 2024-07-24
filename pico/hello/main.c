#include <stdio.h>
#include "pico/stdlib.h"
#include "xlog.h"

void pin_configure()
{
    const uint WIFI_BOARD_EN_PIN = 16;

    gpio_init(WIFI_BOARD_EN_PIN);
    gpio_set_dir(WIFI_BOARD_EN_PIN, GPIO_OUT);
    gpio_put(WIFI_BOARD_EN_PIN, 1);
}

int main() 
{
    stdio_init_all();

    pin_configure();

    xlog_dbg("hello\n");
    return 0;
}