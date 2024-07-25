#include <stdio.h>
#include "pico/stdlib.h"
#include "xlog.h"
#include "TinyFrame.h"

typedef enum {
    MSG_TYPE_ECHO = 0x0,
} MSG_TYPE;

static TinyFrame s_tf_;
static TinyFrame *s_tf = &s_tf_;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    uart_write_blocking(uart1, buff, len);
}

static void pin_configure()
{
    const uint WIFI_BOARD_EN_PIN = 16;

    gpio_init(WIFI_BOARD_EN_PIN);
    gpio_set_dir(WIFI_BOARD_EN_PIN, GPIO_OUT);
    gpio_put(WIFI_BOARD_EN_PIN, 1);
}

static void uart_configure()
{
    uart_init(uart1, 115200);
    gpio_set_function(6, GPIO_FUNC_UART);
    gpio_set_function(7, GPIO_FUNC_UART);
}

static TF_Result my_tf_echo_listener(TinyFrame *tf, TF_Msg *msg)
{
    xlog_dbg("Got response: %s", msg->data);
    return TF_CLOSE;
}

int main() 
{
    stdio_init_all();
    xlog_dbg("hello\n");

    pin_configure();

    TF_InitStatic(s_tf, TF_MASTER);

    while (1) {
        uint8_t msg_data[] = "hello, world!";
        TF_Msg msg;
        TF_ClearMsg(&msg);
        msg.type = MSG_TYPE_ECHO;
        msg.data = msg_data;
        msg.len = sizeof(msg_data);
        TF_Query(s_tf, &msg, my_tf_echo_listener, 20);
        sleep_ms(1000);
    }

    return 0;
}