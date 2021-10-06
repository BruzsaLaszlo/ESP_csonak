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
#define rxPin 14
#define txPin 12
SoftwareSerial sim800(rxPin,txPin);
TinyGsm modem(sim800);

#define SSL_PORT 443
#define DATABASE_URL "https://steg-esp8266.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, DATABASE_URL, SSL_PORT);

void inicSIM800L()
{
    sim800.begin(9600);
    Serial.println("SIM800L serial initialize");

    //Restart takes quite some time
    //To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    modem.init();
    String modemInfo = modem.getModemInfo();
    Serial.print("Modem: ");
    Serial.println(modemInfo);

    // Unlock your SIM card with a PIN
    //modem.simUnlock("1234");

    http_client.setHttpResponseTimeout(5 * 1000); //^0 secs timeout
}