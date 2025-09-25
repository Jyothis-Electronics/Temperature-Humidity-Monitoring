  #include <ESP8266WiFi.h>
#include <DHT.h>

// === WiFi Configuration ===
const char* ssid = "OneplusNord";        // 🔑 Replace with your Wi-Fi name
const char* password = "NordEffect"; // 🔑 Replace with your Wi-Fi password

// === DHT11 Configuration ===
#define DHTPIN D4      // NodeMCU D4 = GPIO2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// === Web Server ===
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // 👈 Note this IP!

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  // Read request
  while (client.connected() && !client.available()) delay(1);
  String req = client.readStringUntil('\r');
  client.flush();

  // Read sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check for sensor errors
  if (isnan(humidity) || isnan(temperature)) {
    humidity = -1;
    temperature = -1;
  }

  // Build HTML response
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<style>"
                "body { font-family: Arial; text-align: center; margin-top: 50px; }"
                ".card { background: #f0f8ff; padding: 20px; border-radius: 10px; display: inline-block; }"
                ".value { font-size: 24px; color: #007bff; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='card'>"
                "<h2>🌡️ DHT11 Sensor</h2>";

  if (temperature >= 0) {
    html += "<p>Temperature: <span class='value'>" + String(temperature, 1) + "°C</span></p>";
    html += "<p>Humidity: <span class='value'>" + String(humidity, 1) + "%</span></p>";
  } else {
    html += "<p style='color:red;'>❌ Sensor Error!</p>";
  }

  html += "<p>IP: " + WiFi.localIP().toString() + "</p>"
          "</div>"
          "</body>"
          "</html>";

  // Send HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println(); // End headers
  client.println(html);
  delay(100);
}


