//Select your modem
//SSL/TLS is currently supported only with SIM8xx series
#define TINY_GSM_MODEM_SIM800
//Increase RX buffer
#define TINY_GSM_RX_BUFFER 256
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <SoftwareSerial.h>

char apn[] = "internet.telekom";
char user[] = "";
char pass[] = "";

// !!!!!!!!!!!!!!!!!!!!!!!!!!!1
#define rxPin 4
#define txPin 2
SoftwareSerial sim800(rxPin,txPin);
TinyGsm modem(sim800);

TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, DATABASE_URL, SSL_PORT);

unsigned long previousMillis = 0;
long interval = 10000;

void inic()
{
    sim800.begin(9600);
    Serial.println("SIM800L serial initialize");

    //Restart takes quite some time
    //To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    modem.restart();
    String modemInfo = modem.getModemInfo();
    Serial.print("Modem: ");
    Serial.println(modemInfo);

    // Unlock your SIM card with a PIN
    //modem.simUnlock("1234");

    http_client.setHttpResponseTimeout(90 * 1000); //^0 secs timeout
}