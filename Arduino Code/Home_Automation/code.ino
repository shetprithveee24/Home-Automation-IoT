#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Define the LED and PIR pin
#define RED_LED D2    // GPIO4 (Red LED)
#define GREEN_LED D0  // GPIO16 (Green LED)
#define PIR_PIN D5    // GPIO14 (PIR Sensor Output)
#define NEW_LED D4    // GPIO2 (New LED)

// Wi-Fi credentials
const char* ssid = "Prithvi";
const char* password = "chingchong12";

// Web server
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Set LED pins as output
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(NEW_LED, OUTPUT);
  pinMode(PIR_PIN, INPUT);  // Set PIR sensor pin as input

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(NEW_LED, LOW); // Start with new LED off

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Webpage - LED Control UI
  server.on("/", []() {
    String html = "<html><body><h2>ESP8266 Dual LED Control</h2>";
    html += "<p>Device IP: " + WiFi.localIP().toString() + "</p>";

    html += "<h3>🔴 Red LED (D2)</h3>";
    html += "<button onclick=\"fetch('/red/on')\">Turn RED ON</button> ";
    html += "<button onclick=\"fetch('/red/off')\">Turn RED OFF</button><br><br>";

    html += "<h3>🟢 Green LED (D0)</h3>";
    html += "<button onclick=\"fetch('/green/on')\">Turn GREEN ON</button> ";
    html += "<button onclick=\"fetch('/green/off')\">Turn GREEN OFF</button><br><br>";

    html += "<h3>🟠 New LED (D4)</h3>";
    html += "<button onclick=\"fetch('/newled/on')\">Turn NEW LED ON</button> ";
    html += "<button onclick=\"fetch('/newled/off')\">Turn NEW LED OFF</button><br><br>";

    html += "<button onclick=\"fetch('/status').then(r => r.text()).then(alert)\">Check Status</button>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // Red LED routes
  server.on("/red/on", HTTP_GET, []() {
    digitalWrite(RED_LED, HIGH);
    server.send(200, "text/plain", "Red LED ON");
  });

  server.on("/red/off", HTTP_GET, []() {
    digitalWrite(RED_LED, LOW);
    server.send(200, "text/plain", "Red LED OFF");
  });

  // Green LED routes
  server.on("/green/on", HTTP_GET, []() {
    digitalWrite(GREEN_LED, HIGH);
    server.send(200, "text/plain", "Green LED ON");
  });

  server.on("/green/off", HTTP_GET, []() {
    digitalWrite(GREEN_LED, LOW);
    server.send(200, "text/plain", "Green LED OFF");
  });

  // New LED routes
  server.on("/newled/on", HTTP_GET, []() {
    digitalWrite(NEW_LED, HIGH);
    server.send(200, "text/plain", "New LED ON");
  });

  server.on("/newled/off", HTTP_GET, []() {
    digitalWrite(NEW_LED, LOW);
    server.send(200, "text/plain", "New LED OFF");
  });

  // Status route
  server.on("/status", HTTP_GET, []() {
    String status = "LED Status:\n";
    status += "Red LED: " + String(digitalRead(RED_LED) ? "ON" : "OFF") + "\n";
    status += "Green LED: " + String(digitalRead(GREEN_LED) ? "ON" : "OFF") + "\n";
    status += "New LED: " + String(digitalRead(NEW_LED) ? "ON" : "OFF");
    server.send(200, "text/plain", status);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Check PIR sensor to turn the new LED on/off
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    // Motion detected - Turn the new LED ON
    digitalWrite(NEW_LED, HIGH);  // Turn the new LED ON
  } else {
    // No motion detected - Turn the new LED OFF
    digitalWrite(NEW_LED, LOW);   // Turn the new LED OFF
  }
}
