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
    return TF_STAY;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        TF_AcceptChar(s_tf, inChar);
    }
}

void setup() {
    Serial.begin(115200);

    if (TF_InitStatic(s_tf, TF_MASTER)) {
        Serial.println("Tf init successful\n");
    } else {
        Serial.println("Tf init failed\n");
    }
    
    TF_AddGenericListener(s_tf, my_tf_generic_listener);
}

void loop() {

}