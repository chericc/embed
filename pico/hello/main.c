#include <stdio.h>
#include "pico/stdlib.h"
#include "xlog.h"
#include "TinyFrame.h"

typedef enum {
    MSG_TYPE_ECHO = 0x0,
} MSG_TYPE;

static TinyFrame s_tf_;
static TinyFrame *s_tf = &s_tf_;

#define DEBUG_UART  uart1
#define COMM_UART   uart0

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    for (int i = 0; i < len; ++i) {
        uart_putc(COMM_UART, buff[i]);
    }
    hex_dump("send", buff, len);
}

static void pin_configure()
{
    const uint WIFI_BOARD_EN_PIN = 16;

    gpio_init(WIFI_BOARD_EN_PIN);
    gpio_set_dir(WIFI_BOARD_EN_PIN, GPIO_OUT);
    gpio_put(WIFI_BOARD_EN_PIN, 1);

    const uint WIFI_BOARD_RST_PIN = 17;

    gpio_init(WIFI_BOARD_RST_PIN);
    gpio_set_dir(WIFI_BOARD_RST_PIN, GPIO_OUT);
    gpio_put(WIFI_BOARD_RST_PIN, 1);
}

static void on_uart_comm(void)
{
    while (uart_is_readable(COMM_UART)) {
        uint8_t ch = uart_getc(COMM_UART);
        // xlog_dbg("COMM_UART got: %x", ch);
        TF_AcceptChar(s_tf, ch);
    }
}

static void uart_configure()
{
    uart_init(COMM_UART, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_set_hw_flow(COMM_UART, false, false);
    uart_set_format(COMM_UART, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(COMM_UART, false);

    irq_set_exclusive_handler(UART0_IRQ, on_uart_comm);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, 0);
}

static TF_Result my_tf_echo_listener(TinyFrame *tf, TF_Msg *msg)
{
    xlog_dbg("Got response: %s", msg->data);
    return TF_STAY;
}

int main() 
{
    stdio_uart_init_full(uart1, PICO_DEFAULT_UART_BAUD_RATE, 8, 9);

    xlog_dbg("hello");

    pin_configure();

    uart_configure();

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
        sleep_ms(5000);
    }

    return 0;
}