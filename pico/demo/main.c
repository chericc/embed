#include <stdio.h>
#include "pico/stdlib.h"
#include "xlog.h"

int main()
{
    stdio_init_all();
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(800);
        gpio_put(LED_PIN, 0);
        sleep_ms(200);
        xlog_dbg("run");
    }
    return 0;
}