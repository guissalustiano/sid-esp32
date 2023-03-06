#include <Arduino.h>
#include <WiFi.h>

# define LED 18

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "SalustRedmi"
#define WIFI_PASSWORD "12qw12qw"

/* 2. Define the API Key */
#define API_KEY "AIzaSyD0Tvf7tbGvfSQEOM9zc_TfSiq1L6HPke8"

/* 2. Define the RTDB URL */
#define DATABASE_URL "morte-a-labdig.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app


/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "morte-a-labdig"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

unsigned long dataMillis = 0;

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    // config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    config.signer.test_mode = true;

    /* Assign the user sign in credentials */
    // auth.user.email = USER_EMAIL;
    // auth.user.password = USER_PASSWORD;

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.

    /* Assign the callback function for the long running token generation task */
    // config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
}

void loop()
{

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0))
    {
        dataMillis = millis();

        String documentPath = "data/led";
        String mask = "value";

        // If the document path contains space e.g. "a b c/d e f"
        // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

        Serial.print("Get a document... ");

        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
            Serial.println(fbdo.errorReason());
    }
}