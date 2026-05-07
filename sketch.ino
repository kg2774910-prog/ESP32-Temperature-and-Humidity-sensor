#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN 4 // GPIO4
#define DHTTYPE DHT22 // Change to DHT22 if using that sensor

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* apiKey = "FZ3609DSHC06TW9X";
unsigned long channelID = 3363272;

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor read failed!");
    return;
  }

  Serial.printf("Temp: %.1f C Humidity: %.1f %%\n", t, h);

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int code = ThingSpeak.writeFields(channelID, apiKey);
  Serial.println(code == 200 ? "Sent OK" : "Send failed: " + String(code));

  delay(15000); // ThingSpeak free tier: min 15s; 10s is fine for testing
}