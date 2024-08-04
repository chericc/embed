
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

extern "C"
{
#include "src/tinyframe/TinyFrame.h"
#include "src/mjson/mjson.h"
}

/*

{
    "ssid": "Mi_24",
    "password": "12345678"
}

{
    // 0: connect successful
    // 1: connecting
    // 2: connect timeout
    // -1: error
    "code": 0,
    "msg": ""
}

*/

typedef enum {
    MSG_TYPE_ECHO = 0x0,
    MSG_TYPE_WIFI_CFG = 0x1,
} MSG_TYPE;

typedef struct {
    char ssid[64];
    char password[64];
} WifiConfig;

typedef struct {
    int code;
    char msg[64];
} WifiResponse;

typedef struct {

} MainConfig;

static TinyFrame tf;
static TinyFrame *s_tf = &tf;

MainConfig g_main_cfg;

void TF_WriteImpl(TinyFrame *tf, const uint8_t *buff, uint32_t len)
{
    for (int i = 0; i < len; ++i) {
      while (!Serial.availableForWrite());
      Serial.write(buff[i]);
    }
}

static int wifi_response(TinyFrame *tf, const WifiResponse *response)
{
    TF_Msg msg;
    TF_ClearMsg(&msg);
    char json_msg[256];
    mjson_snprintf(json_msg, sizeof(json_msg), "{%Q:%d,%Q:%Q}", "code", response->code, "msg", response->msg);
    msg.data = (uint8_t *)json_msg;
    msg.len = strlen(json_msg) + 1;
    TF_Send(tf, &msg);
    return 0;
}

static int handle_msg_wifi_config(TinyFrame *tf, TF_Msg *msg)
{
    int error_flag = 0;
    WifiResponse response = { 0 };

    do {
        int ret = 0;

        char ssid[64];
        char password[64];

        Serial.println("wifi config");

        // parse wifi.ssid
        ret = mjson_get_string((const char*)msg->data, msg->len, "$.ssid", ssid, sizeof(ssid));

        Serial.printf("get ssid: ssid=%s, ret=%d\n", ssid, ret);

        if (ret < 0) {
            error_flag = 1;
            snprintf(response.msg, sizeof(response.msg), "%s", "get ssid failed");
            break;
        }

        response.code = 1;
        snprintf(response.msg, sizeof(response.msg), "%s", "get ssid successful");
        wifi_response(tf, &response);

        Serial.printf("response end");

        // parse wifi.password
        ret = mjson_get_string((const char *)msg->data, msg->len, "$.password", password, sizeof(password));
        if (ret < 0) {
            error_flag = 1;
            snprintf(response.msg, sizeof(response.msg), "%s", "get password failed");
            break;
        }

        response.code = 1;
        snprintf(response.msg, sizeof(response.msg), "%s", "get password successful");
        wifi_response(tf, &response);

        Serial.printf("ssid:<%s>, password:<%s>\n", ssid, password);

        const int max_count = 5;
        int count = 0;
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        
        while (true) {
            wl_status_t status = WiFi.status();
            Serial.printf("status: %d\n", (int)status);
            if (status == WL_CONNECTED) {
                Serial.printf("connected\n");
                break;
            }
            ++count;
            delay(1000);
            response.code = 1;
            snprintf(response.msg, sizeof(response.msg), "%s", "connecting: %d(%d)", count, max_count);
            wifi_response(tf, &response);
            if (count >= max_count) {
                error_flag = 1;
                response.code = -1;
                snprintf(response.msg, sizeof(response.msg), "%s", "connect timeout");
                break;
            }
        }

        response.code = 0;


    } while (0);

    if (error_flag) {
        response.code = -1;
        wifi_response(tf, &response);
    }

    return error_flag ? -1 : 0;
}

static TF_Result my_tf_generic_listener(TinyFrame *tf, TF_Msg *msg)
{
    switch (msg->type) {
        case MSG_TYPE_ECHO:
        {
            TF_Respond(s_tf, msg);
            break;
        }
        case MSG_TYPE_WIFI_CFG:
        {
            handle_msg_wifi_config(tf, msg);
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

    {
        TF_Msg msg;
        TF_ClearMsg(&msg);
        char str_msg[128];

        snprintf(str_msg, sizeof(str_msg), "{ \"ssid\": \"Mi24\", \"password\": \"20220101\" }");
        msg.data = (uint8_t*)str_msg;
        msg.len = strlen(str_msg) + 1;

        handle_msg_wifi_config(s_tf, &msg);
    }
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

