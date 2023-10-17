#include <../lib/DataStruct.h>
#include "Firebase.h"

//const char FIREBASE_HOST[] = "steg-esp8266.firebaseio.com";
const char FIREBASE_AUTH[] = "b7e2ec7d4e60396833d06d2997ebab620888484e";

const char PATH_WARNING_SENSORS[20] = "/sensors_warning/";

// Firebase Realtime Database Object
FirebaseData firebaseData;
// Firebase Authentication Object
FirebaseAuth auth;
// Firebase configuration Object
FirebaseConfig config;


void setupFirebase() {
    // web api key
    config.api_key = "AIzaSyBef5pt59U8Ww2YWhTE4XycPPH67mg3u4A";
    config.database_url = "https://steg-esp8266.firebaseio.com";
    auth.token.uid = "riPOhDWdHJdXGUhZc8R7MeXLp813";
    auth.user.email = "valaki@server.huu";
    auth.user.password = "valamipass";

    /* Assign the callback function for the long running token generation task */
    // TODO
    TokenStatusCallback tokenStatusCallback;
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h


    // Initialise the firebase library
    Firebase.begin(&config, &auth);
    const char *path = "login/valaki";
    const char *value = "success";
    Firebase.setString(firebaseData, path, value);
}

void setFirebase(GpsDataStruct data) {
    char path[127];
    sprintf(path, "/GPS/%d/%d/%d", data.year, data.month, data.day);
    Firebase.setDouble(firebaseData, path, data.lat);
    Firebase.setDouble(firebaseData, path, data.lon);
    Firebase.setDouble(firebaseData, path, data.speed);
}

void setFirebase() {

}

