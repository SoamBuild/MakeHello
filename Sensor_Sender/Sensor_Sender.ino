#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include "paj7620.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN D4
#define LED_COUNT 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// MacXdevices
uint8_t device1[] = {0x84, 0xCC, 0xA8, 0xA2, 0xF7, 0x33};
uint8_t device2[] = {0xBC, 0xDD, 0xC2, 0xB6, 0x29, 0xC2};
// DataXsent
typedef struct msg
{
  bool state;
} msg;

msg myData;

boolean left_CHECK = false;
boolean right_CHECK = false;

// CallbackXsent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Ultima alerta: ");
  if (sendStatus == 0)
  {
    Serial.println("recibida");
  }
  else
  {
    Serial.println("Perdida");
  }
}

void setup()
{
  // Serial monitor
  Serial.begin(115200);
  // Init Sensor Gesto
  paj7620Init();

  strip.begin();
  strip.show();

  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("# Error al iniciar ESPNOW");
    return;
  }
  else
  {
    Serial.println("# ESPNOW OK");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Receptores
  esp_now_add_peer(device1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(device2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  onboot_Mode(0, 165, 0);
}

void loop()
{

  uint8_t data = 0; // Read Bank_0_Reg_0x43/0x44 for gesture result.
  paj7620ReadReg(0x43, 1, &data);
  if (data == GES_RIGHT_FLAG)
  {
    Serial.println("RIGHT");
    standby_Mode(2, 59, 100, 4, 8);
    right_CHECK = true;
  }

  if (data == GES_LEFT_FLAG)
  {
    Serial.println("LEFT");
    left_CHECK = true;
    standby_Mode(2, 59, 100, 0, 4);
  }
  if (right_CHECK == true && left_CHECK == true)
  {
    MakeHello();
  }
}
void MakeHello()
{
  myData.state = true;
  alert_Mode(80, 80, 80, 20);
  esp_now_send(device1, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(device2, (uint8_t *)&myData, sizeof(myData));
  delay(10000);
  strip.clear();
  strip.show();
  MakeGoodbye();
}
void MakeGoodbye()
{
  left_CHECK = false;
  right_CHECK = false;
  myData.state = false;
  esp_now_send(device1, (uint8_t *)&myData, sizeof(myData));
  esp_now_send(device2, (uint8_t *)&myData, sizeof(myData));
  
}
void standby_Mode(int r, int g, int b, int min, int max)
{
  for (int i = 40; i <= 80; i++)
  {
    strip.fill(strip.Color(r, g, b), min, max);
    strip.setBrightness(i);
    strip.show();
    delay(20);
  }
  for (int i = 80; i >= 40; i--)
  {
    strip.fill(strip.Color(r, g, b), min, max);
    strip.setBrightness(i);
    strip.show();
    delay(10);
  }
}
void onboot_Mode(int r, int g, int b)
{
  for (int i = 0; i <= 8; i++)
  {
    strip.setPixelColor(i, r, g, b);
    strip.show();
    delay(1000);
  }
  delay(100);
  for (int i = 8; i >= 0; i--)
  {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(1000);
  }
  strip.show();
  strip.clear();
}
void alert_Mode(int r, int g, int b, int wait)
{
  strip.show();
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i++)
  {                                  // For each pixel in strip...
    strip.setPixelColor(i, r, g, b); //  Set pixel's color (in RAM)
    strip.show();                    //  Update strip to match
    delay(wait);                     //  Pause for a moment
  }
}