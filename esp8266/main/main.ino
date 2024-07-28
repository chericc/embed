extern "C"
{
#include "src/tinyframe/TinyFrame.h"
}

typedef enum {
    MSG_TYPE_ECHO = 0x0,
} MSG_TYPE;

static TinyFrame tf;
static TinyFrame *s_tf = &tf;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    for (int i = 0; i < len; ++i) {
      while (!Serial.availableForWrite());
      Serial.write(buff[i]);
    }
}

static TF_Result my_tf_generic_listener(TinyFrame *tf, TF_Msg *msg)
{
    switch (msg->type) {
        case MSG_TYPE_ECHO:
        {
            TF_Respond(s_tf, msg);
            break;
        }
        default:
        {
            TF_Respond(s_tf, msg);
            break;
        }
    }
    return TF_STAY;
}

void setup() {
    Serial.begin(115200);
    Serial.setRxBufferSize(256);

    Serial.printf("\n");
    Serial.printf("\n");

    if (TF_InitStatic(s_tf, TF_MASTER)) {
        Serial.println("Tf init successful\n");
    } else {
        Serial.println("Tf init failed\n");
    }
    
    TF_InitStatic(s_tf, TF_MASTER);
    TF_AddGenericListener(s_tf, my_tf_generic_listener);
}

void loop() {
    // Serial.
    int available = Serial.available();

    if (available > 0) {
      uint8_t buffer[256];
      int ret = Serial.read(buffer, available);
      TF_Accept(s_tf, buffer, ret);
    }
    else {
      delay(5); // 115200 --> 128 Bytes / 10ms
    }
}

