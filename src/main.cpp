#include <Arduino.h>
#include <ArduinoOTA.h>

IPAddress gateWay;
#include <WifiConnect.h>

const int PORT = 9091;

WiFiClient client;

uint8_t csorlo, led1, led2;
const uint8_t ELORE_PIN = 0;
const uint8_t HATRA_PIN = 2;
const uint8_t LED1 = 22;
const uint8_t LED2 = 23;
#define LED 2 //Define blinking LED pin


void setup()
{
  Serial.begin(9600);
  Serial.println("Hello!");
  connectWifi(0);
  pinMode(ELORE_PIN, OUTPUT);
  pinMode(HATRA_PIN, OUTPUT);
 // pinMode(LED1, OUTPUT); // Initialize the LED pin as an output
  digitalWrite(ELORE_PIN, LOW);
  digitalWrite(HATRA_PIN, LOW);
  //ArduinoOTA.begin();
}

uint8_t b[] = {0, 0, 0};
uint8_t *bb = b;

void loop()
{
  //ArduinoOTA.handle();

  if (client.connect(gateWay, PORT))
  {
    client.println("ESP8266 connected!");
    if (client.readBytes(bb, 3) != 3)
    {
      b[0] = 0;
    }
    client.stop();
  }
  else
  {
    client.stop();
  }
  if (b[0] == 0)
  {
    digitalWrite(ELORE_PIN, LOW);
    digitalWrite(HATRA_PIN, LOW);
  }
  else if (b[0] == 1)
  {
    digitalWrite(HATRA_PIN, LOW);
    delay(10);
    digitalWrite(ELORE_PIN, HIGH);
  }
  else if (b[0] == 2)
  {
    digitalWrite(ELORE_PIN, LOW);
    delay(10);
    digitalWrite(HATRA_PIN, HIGH);
  }
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
}
