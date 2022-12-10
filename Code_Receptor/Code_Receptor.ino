#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <espnow.h>

#define LED_PIN D3
#define LED_COUNT 3

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

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
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  myData.recep_alert = false;

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
  Serial.println(myData.recep_alert);
  if (myData.recep_alert == true)
  {
    alert_Mode(127, 0, 0, 50);
  }
  else
  {
    standby_Mode(2, 59, 100);
  }
}
void standby_Mode(int r, int g, int b)
{
  for (int i = 40; i <= 80; i++)
  {
    strip.fill(strip.Color(r, g, b), 0, 3);
    strip.setBrightness(i);
    strip.show();
    delay(20);
  }
  for (int i = 80; i >= 40; i--)
  {
    strip.fill(strip.Color(r, g, b), 0, 3);
    strip.setBrightness(i);
    strip.show();
    delay(10);
  }
}
void alert_Mode(int r, int g, int b, int wait)
{
  for (int a = 0; a < 10; a++)
  {
    for (int b = 0; b < 3; b++)
    {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        strip.setPixelColor(c, r, g, b);
      }
      strip.show();
      delay(wait);
    }
  }
}