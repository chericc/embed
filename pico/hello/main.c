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
    for (int i = 0; i < len; ++i) {
        xlog_dbg("uart1 send: %x", buff[i]);
        uart_putc(uart1, buff[i]);
    }
    xlog_dbg("uart1 send %d bytes done", len);
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
    gpio_set_function(5, GPIO_FUNC_UART);
    gpio_set_function(6, GPIO_FUNC_UART);
}

static void irq_handler_uart1(void)
{
    while (uart_is_readable(uart1)) {
        uint8_t ch = uart_getc(uart1);
        xlog_dbg("uart1 got: %x", ch);
        TF_AcceptChar(s_tf, ch);
    }
}

static void interrupt_configure()
{
    irq_set_exclusive_handler(UART1_IRQ, irq_handler_uart1);
    uart_set_irq_enables(uart1, true, 0);
}

static TF_Result my_tf_echo_listener(TinyFrame *tf, TF_Msg *msg)
{
    xlog_dbg("Got response: %s", msg->data);
    return TF_STAY;
}

int main() 
{
    stdio_init_all();
    xlog_dbg("hello");

    pin_configure();

    uart_configure();

    interrupt_configure();

    TF_InitStatic(s_tf, TF_MASTER);

    TF_AddGenericListener(s_tf, my_tf_echo_listener);

    while (1) {
        uint8_t msg_data[] = "hello, world!";
        TF_Msg msg;
        TF_ClearMsg(&msg);
        msg.type = MSG_TYPE_ECHO;
        msg.data = msg_data;
        msg.len = sizeof(msg_data);
        TF_Send(s_tf, &msg);
        sleep_ms(1000);
    }

    return 0;
}