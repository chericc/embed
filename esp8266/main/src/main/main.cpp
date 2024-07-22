#include "main.h"


#include <Arduino.h>
#include <pins_arduino.h>

extern "C"
{
#include "../tinyframe/TinyFrame.h"
}

typedef enum {
    MSG_TYPE_ECHO = 0x0,
} MSG_TYPE;

static TinyFrame tf;
static TinyFrame *s_tf = &tf;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    Serial.write(buff, len);
}

static TF_Result my_tf_generic_listener(TinyFrame *tf, TF_Msg *msg)
{
    switch (msg->type) {
        case MSG_TYPE_ECHO:
        {
            TF_Respond(tf, msg);
            break;
        }
        default:
        {
            break;
        }
    }
    return TF_CLOSE;
}

int custom_main()
{
    Serial.begin(115200);
    while(!Serial) {
        ;
    }

    if (TF_InitStatic(s_tf, TF_MASTER)) {
        Serial.println("Tf init successful\n");
    } else {
        Serial.println("Tf init failed\n");
    }

    TF_AddGenericListener(s_tf, my_tf_generic_listener);

    uint8_t buffer[64];
    while (true) {
        int ret_read = Serial.read(buffer, sizeof(buffer));
        if (ret_read > 0) {
            TF_Accept(s_tf, buffer, ret_read);
        } else {
            delay(1);
        }
    }
}