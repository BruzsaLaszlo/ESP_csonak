#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <firebase.h>

#define TEST_SIM800 FALSE

#if TEST_SIM800
#include <gprs.h>
#else
#include <sim800_test.h>
#endif

IPAddress gateWay;
#include <WifiConnect.h>
// Port
const int PORT = 9091;

WiFiClient client;

#define PINS_D 11
#define PINS_A 6

#define ELORE_PIN 4
#define HATRA_PIN 7
#define RELAY1 8
#define RELAY2 12
#define SIZE_LEDS 6
const uint8_t LEDS_PINS[] = {3, 5, 6, 9, 10, 11};
uint8_t leds[SIZE_LEDS];

#define SIZE_B 9
uint8_t b[SIZE_B];
//uint8_t *bb = b;
unsigned long t = millis();
boolean unoConnected = false;

#define FIRST_FLAG 100

#include <Wire.h>
#define WAVGAT_UNO 4
#define NODEMCU 44

void setPins()
{
    Wire.beginTransmission(WAVGAT_UNO); // transmit to device #4
    {
        // Csörlő
        {
            if (b[0] == 1)
            {
                Wire.write(ELORE_PIN);
                Wire.write(HIGH);
                Wire.write(HATRA_PIN);
                Wire.write(LOW);
            }
            else if (b[0] == 2)
            {
                Wire.write(ELORE_PIN);
                Wire.write(LOW);
                Wire.write(HATRA_PIN);
                Wire.write(HIGH);
            }
            else
            {
                Wire.write(ELORE_PIN);
                Wire.write(LOW);
                Wire.write(HATRA_PIN);
                Wire.write(LOW);
            }
        }
        // LED
        {
            for (int i = 0; i < SIZE_LEDS; i++)
            {
                Wire.write(LEDS_PINS[i]);
                Wire.write(leds[i]);
            }
        }
        // RELAYs
        {
            Wire.write(RELAY1);
            Wire.write(LOW);
            Wire.write(RELAY2);
            Wire.write(LOW);
        }
    }
    Wire.endTransmission(); // stop transmitting
}

//void(* resetFunc) (void) = 0; //declare reset function @ address 0
//void reset() { asm volatile ("jmp 0"); }
//ESP.reset();

void setup()
{

    Serial.begin(115200);

    Serial.println("/nProgram started!");

    b[0] = FIRST_FLAG;
    EEPROM.begin(8);
    for (int i = 0; i < 6; i++)
    {
        leds[i] = EEPROM.read(i);
        b[i + 1] = leds[i];
    }

    Wire.begin();
    delay(100);
    setPins();

    connectWifi(0);

    client.setTimeout(300);
    ArduinoOTA.begin();
}

void tt()
{
    Serial.print(millis() - t);
    Serial.println("ms");
    t = millis();
}

boolean test = false;

void logPhone(char *out)
{
    if (client.connect(gateWay, PORT))
    {
        client.println("Serial.monitor");

        client.println(out);
        Serial.println(out);

        client.println("END");
        client.stop();
    }
}

char scc[70] = {};

void loop()
{
    delay(50);

    ArduinoOTA.handle();

    if (client.connect(gateWay, PORT))
    {
        // Message
        char m[50] = {};

        client.println("ESP8266: Connected!");

        //client.readStringUntil('Q');
        b[0] = atoi(client.readStringUntil('B').c_str());
        if (b[0] == 100)
        {
            /*test = client.readStringUntil('S') == "true" ? true : false;
            Serial.println("first");
            Serial.println(test);*/
            b[0] = 0;
            char c[4];
            for (int i = 0; i < SIZE_LEDS; i++)
            {
                sprintf(c, "%d", leds[i]);
                client.println(c);
            }
            client.stop();
        }
        else if (client.available() > 0)
        {
            for (int i = 1; i < SIZE_B; i++)
                b[i] = atoi(client.readStringUntil('B').c_str());

            boolean l = false;
            for (int i = 0; i < 6; i++)
                if (b[i + 1] != leds[i])
                {
                    leds[i] = b[i + 1];
                    EEPROM.write(i, leds[i]);
                    l = true;
                }
            if (l && EEPROM.commit())
                strcat(m, "EEPROM-ba elmentve");
        }
        client.stop();
        if (strlen(m) > 0)
            logPhone(m);
    }
    else
    {
        b[0] = 0;
        client.stop();
    }

    setPins();

    // a kapcsolat tesztelese (Wavgat))
    {
        //MAX 128 bytes
        uint8_t requestSize = Wire.requestFrom(WAVGAT_UNO, (PINS_A));
        if (requestSize == 0 && unoConnected)
        {
            unoConnected = false;
            logPhone((char *)"Nincs meg az UNO!!!!");
        }
        if (requestSize > 0 && !unoConnected)
        {
            unoConnected = true;
            logPhone((char *)"Megvan az UNO!!!!");
        }
        //
        while (Wire.available() > 0)
            Wire.read();
    }

    // get GPS data
    /*{
        gpsDataStruct data;

        int numbytes = sizeof(data);
        int n = Wire.requestFrom(2, numbytes);
        if (n == numbytes) // check if the same amount received as requested
        {
            Wire.readBytes((char *)&data, n);
        }

        String s = getJSONgpsData(data);
        postToFirebase("GPSdata",s,&http_client);
    }*/

    if (test) // test
    {
        logPhone(setupFirebase());
        logPhone(testFirebase());

        //testSIM800();
        //inicSIM800L();
        //postToFirebase("/GPRS/probasikeres", "TRUE", &http_client);
        test = false;
    }

    /* SERIAL PRINT */
    char sc[70] = {'\n'};
    char cc[5] = {};
    for (int i = 0; i < SIZE_B; i++)
    {
        sprintf(cc, "%d ", b[i]);
        strcat(sc, cc);
    }

    unsigned long ct = millis();

    if (strcmp(scc, sc) != 0 || ct - t > 100)
    {
        strcpy(scc, sc);
        Serial.print(sc);
        Serial.printf("%lu ms\n", ct - t);
    }
    else
        Serial.print('^');
    t = millis();
}
