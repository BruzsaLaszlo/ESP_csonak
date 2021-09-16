#include <DataStruct.h>

#define FIREBASE_HOST "steg-esp8266.firebaseio.com"
#define FIREBASE_AUTH "Z90EaeXMUGTk33qNzgVSN72UXT8xl6OiBilppOEE"
#include <FirebaseESP8266.h>
const char PATH_WARNING_SENSORS[20] = "/sensors_warning/";
FirebaseData firebaseData;
char path[127];

void setupFirebase()
{
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void setFirebase(gpsDataStruct data)
{
    sprintf(path, "/GPS/%d/%d/%d", data.year,data.month,data.day);
    Firebase.setFloat(firebaseData, path, data.lat);
    Firebase.setFloat(firebaseData, path, data.lon);
    Firebase.setFloat(firebaseData, path, data.speed);
}

void setFirebase(){

}