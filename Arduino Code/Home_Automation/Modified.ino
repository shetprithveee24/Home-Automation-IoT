#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Pin Definitions
#define IR_PIN D1
#define PIR_PIN D2
#define LED_WHITE D4    // white
#define LED_WHITE2 D5   // white
#define LED_BLUE D6     // blue
#define LED_BLUE2 D7    // blue
#define LED_ORANGE D8   // orange

Servo motor;

// Network Credentials 
const char* ssid = "WiFi - SSID"; // Replace with your Connection Name (SSID)
const char* password = "WiFi - Password"; // Replace with your Connection Password

// Web server instance
ESP8266WebServer server(80);

// NTP Client for timestamps
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 20000); // Update 20 sec

// State Variables
bool doorOpen = false;  // Door state
int prevSensorState = 0; // Previous IR sensor state
unsigned long previousMillis = 0; // Last motion detection time
const long interval = 5000; // Delay for lights
bool pirControlEnabled = true; // Enable/disable PIR-based control
bool whiteLightOn = false;    // White light state
bool pirLightsOn = false;     // Track PIR-controlled light state

// Server URL for logging (replace with your server or Google Sheets web app URL)
const char* logServer = "https://script.google.com/macros/s/AKfycbwR997ZaHiOFmnxVlJ_Kshhsq2r0v9ZIFAHcZrLthm3-icg46YmsXyC5aND2gM_5y7r/exec"; // Update with actual URL

String getTimestamp() {
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    return String(epochTime);
}

void sendLog(String event_type, String light = "", String state = "", String trigger = "") {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        WiFiClient client;
        http.begin(client, logServer);
        http.addHeader("Content-Type", "application/json");
        String timestamp = getTimestamp();
        String payload = "{\"event_type\":\"" + event_type + "\",";
        if (light != "") payload += "\"light\":\"" + light + "\",";
        if (state != "") payload += "\"state\":\"" + state + "\",";
        if (trigger != "") payload += "\"trigger\":\"" + trigger + "\",";
        payload += "\"timestamp\":" + timestamp + "}";
        int httpCode = http.POST(payload);
        if (httpCode > 0) {
            Serial.println("Log sent: " + payload);
        } else {
            Serial.println("Error sending log");
        }
        http.end();
    }
}

void setup() {
    motor.attach(D0);

    pinMode(IR_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_WHITE, OUTPUT);
    pinMode(LED_WHITE2, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_BLUE2, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);

    digitalWrite(LED_WHITE, LOW);
    digitalWrite(LED_WHITE2, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_BLUE2, LOW);
    digitalWrite(LED_ORANGE, LOW);

    // Setup Serial and Wi-Fi
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    // Start NTP client
    timeClient.begin();

    // Define HTTP routes
    server.on("/led_white/on", []() {
        whiteLightOn = true;
        pirControlEnabled = false;
        digitalWrite(LED_WHITE, HIGH);
        digitalWrite(LED_WHITE2, HIGH);
        sendLog("light", "white", "on", "http");
        server.send(200, "text/plain", "White LEDs ON");
    });

    server.on("/led_white/off", []() {
        whiteLightOn = false;
        pirControlEnabled = true;
        digitalWrite(LED_WHITE, LOW);
        digitalWrite(LED_WHITE2, LOW);
        sendLog("light", "white", "off", "http");
        server.send(200, "text/plain", "White LEDs OFF");
    });

    server.on("/led_blue/on", []() {
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(LED_BLUE2, HIGH);
        sendLog("light", "blue", "on", "http");
        server.send(200, "text/plain", "Blue LEDs ON");
    });

    server.on("/led_blue/off", []() {
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_BLUE2, LOW);
        sendLog("light", "blue", "off", "http");
        server.send(200, "text/plain", "Blue LEDs OFF");
    });

    server.on("/led_orange/on", []() {
        digitalWrite(LED_ORANGE, HIGH);
        sendLog("light", "orange", "on", "http");
        server.send(200, "text/plain", "Orange LED ON");
    });

    server.on("/led_orange/off", []() {
        digitalWrite(LED_ORANGE, LOW);
        sendLog("light", "orange", "off", "http");
        server.send(200, "text/plain", "Orange LED OFF");
    });

    server.on("/door/open", []() {
        if (!doorOpen) {
            for (int pos = 0; pos <= 160; pos++) {
                motor.write(pos);
                delay(5);
            }
            doorOpen = true;
            sendLog("door", "", "open", "http");
            server.send(200, "text/plain", "Door Opened");
        } else {
            server.send(200, "text/plain", "Door is already open");
        }
    });

    server.on("/door/close", []() {
        if (doorOpen) {
            for (int pos = 160; pos >= 0; pos--) {
                motor.write(pos);
                delay(5);
            }
            doorOpen = false;
            sendLog("door", "", "close", "http");
            server.send(200, "text/plain", "Door Closed");
        } else {
            server.send(200, "text/plain", "Door is already closed");
        }
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
    timeClient.update();

    // IR Sensor - Servo Motor Control and Entry Logging
    int ir_Sensor = digitalRead(IR_PIN);

    if (ir_Sensor == HIGH && prevSensorState == LOW) {
        for (int pos = 160; pos >= 0; pos--) {
            motor.write(pos);
            delay(5);
        }
        sendLog("entry");
    } else if (ir_Sensor == LOW && prevSensorState == HIGH) {
        for (int pos = 0; pos <= 160; pos++) {
            motor.write(pos);
            delay(5);
        }
        delay(3000);
    }

    prevSensorState = ir_Sensor;

    // PIR Sensor - Turn lights on/off based on motion
    if (pirControlEnabled && !whiteLightOn) {
        int pir_Sensor = digitalRead(PIR_PIN);
        unsigned long currentMillis = millis();

        if (pir_Sensor == HIGH && !pirLightsOn) {
            digitalWrite(LED_WHITE, HIGH);
            digitalWrite(LED_WHITE2, HIGH);
            pirLightsOn = true;
            previousMillis = currentMillis;
            sendLog("light", "white", "on", "pir");
        } else if (pir_Sensor == LOW && pirLightsOn && currentMillis - previousMillis >= interval) {
            digitalWrite(LED_WHITE, LOW);
            digitalWrite(LED_WHITE2, LOW);
            pirLightsOn = false;
            sendLog("light", "white", "off", "pir");
        }
    }
}