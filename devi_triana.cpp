#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
//#include <Firebase32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Library DHT
#include <DHT.h>
#define DHTTYPE DHT11
#define DHT_PIN 4
DHT dht(DHT_PIN, DHTTYPE); // Inisilisasi DHT dengan parameter DHT_PIN dan DHTTYPE

// Sensor LDR
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 1000

#define LDRPIN1 34
#define Y_INTERCEPT_1 3770682801
#define SLOPE_1 -1.93679

#define LDRPIN2 35
#define Y_INTERCEPT_2 22717995.02
#define SLOPE_2 -1.2101

#define LDRPIN3 32
#define Y_INTERCEPT_3 3397895357
#define SLOPE_3 -1.74858

#define LDRPIN4 33
#define Y_INTERCEPT_4 12471824.76
#define SLOPE_4 -1.07894

#define LDRPIN5 25
#define Y_INTERCEPT_5 88613522.37
#define SLOPE_5 -1.39574

#define LDRPIN6 14
#define Y_INTERCEPT_6 300586865.7
#define SLOPE_6 -1.53073

#define LDRPIN7 12
#define Y_INTERCEPT_7 589101325.6
#define SLOPE_7 -1.55183

#define LDRPIN8 13
#define Y_INTERCEPT_8 580390105.5
#define SLOPE_8 -1.64465

#define relayLED 25
#define relayFan 26

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "ikandoyoung"
#define WIFI_PASSWORD "iloveyou"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://smartcomfy-cbca1-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "cSGzBuedhn6dKjWWS2fIoZU2xCFJnbZMOV2QNDpV"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;

void setup()
{
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
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

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    // Or use legacy authenticate method
    // Firebase.begin(DATABASE_URL, DATABASE_SECRET);

    // DHT 11
    dht.begin();

    // LDR
    pinMode(LDRPIN1, INPUT);
    pinMode(LDRPIN2, INPUT);
    pinMode(LDRPIN3, INPUT);
    pinMode(LDRPIN4, INPUT);
    pinMode(LDRPIN5, INPUT);
    pinMode(LDRPIN6, INPUT);
    pinMode(LDRPIN7, INPUT);
    pinMode(LDRPIN8, INPUT);

    // relay
    pinMode(relayLED, OUTPUT);
    pinMode(relayFan, OUTPUT);
    digitalWrite(relayLED, 1);
    digitalWrite(relayFan, 1);
}

void loop()
{
    // buat semua variabel yang ingin diketahui nilainya dan buat nilainya global////////////////////////////
    int Lux1, Lux2, Lux3, Lux4, Lux5, Lux6, Lux7, Lux8;
    String power;

    // baca data kelembapaan dan suhu/////////////////////////////////////////////////////////////////////////
    float suhu = dht.readTemperature();  // memanggil function readTemperature untuk membaca suhu
    float humidity = dht.readHumidity(); // memanggil function readHumidity untuk membaca kelembaban
    if (isnan(suhu) || isnan(humidity))
    {
        Serial.println("Gagal membaca suhu dan kelembaban");
    }
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print("°C || ");
    Serial.print("Kelembaban: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.println();

    // baca semua nilai sensor dan dapatkan nilai lux-nya////////////////////////////////////////////////////
    int LDRValue1 = analogRead(LDRPIN1);
    Lux1 = Y_INTERCEPT_1 * pow(1023 - ((float)LDRValue1 * 0.23) / ((float)LDRValue1 * 0.23), SLOPE_1);
    int LDRValue2 = analogRead(LDRPIN2);
    Lux2 = Y_INTERCEPT_2 * pow(1023 - ((float)LDRValue2 * 0.23) / ((float)LDRValue2 * 0.23), SLOPE_2);
    int LDRValue3 = analogRead(LDRPIN3);
    Lux3 = Y_INTERCEPT_3 * pow(1023 - ((float)LDRValue3 * 0.23) / ((float)LDRValue3 * 0.23), SLOPE_3);
    int LDRValue4 = analogRead(LDRPIN4);
    Lux4 = Y_INTERCEPT_4 * pow(1023 - ((float)LDRValue4 * 0.23) / ((float)LDRValue4 * 0.23), SLOPE_4);
    int LDRValue5 = analogRead(LDRPIN5);
    Lux5 = Y_INTERCEPT_5 * pow(1023 - ((float)LDRValue5) / ((float)LDRValue5), SLOPE_5);
    int LDRValue6 = analogRead(LDRPIN6);
    Lux6 = Y_INTERCEPT_6 * pow(1023 - ((float)LDRValue6) / ((float)LDRValue6), SLOPE_6);
    int LDRValue7 = analogRead(LDRPIN7);
    Lux7 = Y_INTERCEPT_7 * pow(1023 - ((float)LDRValue7) / ((float)LDRValue7), SLOPE_7);
    int LDRValue8 = analogRead(LDRPIN8);
    Lux8 = Y_INTERCEPT_8 * pow(1023 - ((float)LDRValue8) / ((float)LDRValue8), SLOPE_8);

    // print semua data untuk diketahui nilainya///////////////////////////////////////////////////////////
    Serial.print("Lux1: ");
    Serial.print(Lux1);
    Serial.print(" Lux2: ");
    Serial.print(Lux2);
    Serial.print(" Lux3: ");
    Serial.print(Lux3);
    Serial.print(" Lux4: ");
    Serial.print(Lux4);
    Serial.print(" Lux5: ");
    Serial.print(Lux5);
    Serial.print(" Lux6: ");
    Serial.print(Lux6);
    Serial.print(" Lux7: ");
    Serial.print(Lux7);
    Serial.print(" Lux8: ");
    Serial.print(Lux8);
    Serial.println();

    // kirimkan semua data dalam satu kode secara langsung (jangan ada perhitungan disini)///////////////////
    if (Firebase.ready() && (millis() - dataMillis > 5000 || dataMillis == 0))
    {
        Firebase.getString(fbdo, "/SmartComfort/Power");
        power = fbdo.stringData();
        dataMillis = millis();
        Serial.print("Power: ");
        Serial.print(power);
        Serial.println();
        // kirim lux 1//////////////////////////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux1", Lux1))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR2////////////////////////////////////////////////////////////////
        Serial.println();
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux2", Lux2))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR3////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux3", Lux3))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR4////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux4", Lux4))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR5/////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux5", Lux5))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR6/////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux6", Lux6))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR7/////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux7", Lux7))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Reading Light Intensity from LDR8/////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Lux8", Lux8))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // Kirim Data Suhu ke Firebase//////////////////////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Temperature", suhu))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.print("FAILED");
            Serial.print(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
        // kirim data Kelembaban (Humidity) ke firebase//////////////////////////////////////////////////////
        if (Firebase.setFloat(fbdo, "/SmartComfort/Humidity", humidity))
        {
            Serial.print("PASSED");
            Serial.print(" || PATH: " + fbdo.dataPath());
            Serial.print(" || TYPE: " + fbdo.dataType());
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println(" || REASON: " + fbdo.errorReason());
            Serial.println();
        }
    }

    // kontrol relay/////////////////////////////////////////////////////////////////////////////////////////
    // dapatkan nilai relay dari firebase
    if (Firebase.getInt(fbdo, "/relayLED"))
    {
        Serial.print("Get int data A success, data = ");
        Serial.println(fbdo.intData());
        // jika nilai data berhasil didapatkan, baca nilainya dan lakukan gerakan
        // sesuai dengan nilai yang ada
        if (fdbo.intData() == 1)
            digitalWrite(relayLED, HIGH);
        else
            digitalWrite(relayLED, LOW);
    }
    else
    {
        Serial.print("Error in getInt, ");
        Serial.println(fbdo.errorReason());
    }

    if (Firebase.getInt(fbdo, "/relayFan"))
    {
        Serial.print("Get int data B success, data = ");
        Serial.println(fbdo.intData());
        if (fdbo.intData() == 1)
            digitalWrite(relayLED, HIGH);
        else
            digitalWrite(relayLED, LOW);
    }
    else
    {
        Serial.print("Error in getInt, ");
        Serial.println(fbdo.errorReason());
    }
}
