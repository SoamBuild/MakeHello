#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct recep_msg
{

  bool recep_alert;
} recep_msg;

recep_msg myData;

boolean alert = false;

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Bool: ");
  Serial.println(myData.recep_alert);
  alert = myData.recep_alert;
  Serial.println();
}

void setup()
{

  Serial.begin(115200);

  pinMode(D3, OUTPUT);
  digitalWrite(D3, LOW);
  myData.recep_alert =false;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  Serial.println(alert);
  if (myData.recep_alert == true)
  {
    digitalWrite(D3, HIGH);
  }
  else
  {
    digitalWrite(D3, LOW);
  }
}
