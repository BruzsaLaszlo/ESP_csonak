#include <DataStruct.h>
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient

/*#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)*/
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 2. Define the API Key */
#define API_KEY "AIzaSyB5kzZ4sSfGjFPMjWV4rSWwsXYbgmI_yE0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://steg-esp8266.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "lacika007@gmail.com"
#define USER_PASSWORD "Lacika007"

const char PATH_WARNING_SENSORS[20] = "/sensors_warning/";
#define SSL_PORT 443

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

char *out = (char *)malloc(sizeof(char *) * 200);

char *setupFirebase()
{
    sprintf(out, "Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;
    sprintf(out+strlen(out),"API KEY: %s\n",API_KEY);   

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    sprintf(out+strlen(out),"USER EMAIL: %s",USER_EMAIL);
    auth.user.password = USER_PASSWORD;
    sprintf(out+strlen(out),"USER PASSWORD: %s",USER_PASSWORD);

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;
    sprintf(out+strlen(out),"DATABASE_URL: %s",DATABASE_URL);
    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    //Or use legacy authenticate method
    //Firebase.begin(DATABASE_URL, "<database secret>");

    return out;
}

int testInt = 1000;

char *testFirebase()
{
    char c[50] = {};

    strcpy(out, "testFirebase()\n");
    strcat(out, auth.token.uid.c_str());

    String path = auth.token.uid.c_str();
    path += "/testFirebase/testInt";
    if (Firebase.ready())
    {
        sprintf(c, "Set int... %s\n", Firebase.setInt(firebaseData, path, ++testInt) ? "ok" : firebaseData.errorReason().c_str());
        strcat(out, c);

        sprintf(c, "Get int... %s\n", Firebase.getInt(firebaseData, path) ? String(firebaseData.intData()).c_str() : firebaseData.errorReason().c_str());
        strcat(out, c);

        FirebaseJson json;
        json.add("value", testInt);
        json.add("value2", testInt + 300);
        json.add("value3", testInt + 400);

        sprintf(c, "Push json... %s\n", Firebase.pushJSON(firebaseData, path + "/test/push", json) ? "ok" : firebaseData.errorReason().c_str());
        strcat(out, c);

        json.set("value", testInt + 100);
        sprintf(c, "Update json... %s", Firebase.updateNode(firebaseData, String(path + "/test/push/" + firebaseData.pushName()), json) ? "ok" : firebaseData.errorReason().c_str());
        strcat(out, c);
    }
    return out;
}

void setFirebase(char *path, gpsDataStruct data)
{
    sprintf(path, "/GPS/%d/%d/%d", data.year, data.month, data.day);
    Firebase.setFloat(firebaseData, path, data.lat);
    Firebase.setFloat(firebaseData, path, data.lon);
    Firebase.setFloat(firebaseData, path, data.speed);
}

void postToFirebase(const String &path, const String &data, HttpClient *http)
{
    String response;
    int statusCode = 0;
    http->connectionKeepAlive(); // Currently, this is needed for HTTPS

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    String url;
    if (path[0] != '/')
    {
        url = "/";
    }
    url += path + ".json";
    url += "?auth=";
    url += API_KEY;
    Serial.print("POST:");
    Serial.println(url);
    Serial.print("Data:");
    Serial.println(data);
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

    String contentType = "application/json";
    http->put(url, contentType, data);

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // read the status code and body of the response
    //statusCode-200 (OK) | statusCode -3 (TimeOut)
    statusCode = http->responseStatusCode();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    response = http->responseBody();
    Serial.print("Response: ");
    Serial.println(response);
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    if (!http->connected())
    {
        Serial.println();
        http->stop(); // Shutdown
        Serial.println("HTTP POST disconnected");
    }
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
}