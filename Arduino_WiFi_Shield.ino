#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6LFwmB_59"
#define BLYNK_TEMPLATE_NAME "TempMonitor"
#define BLYNK_AUTH_TOKEN "C-QjdroedjYQ5n2X1hGV-kGkf8bOaZm-"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <DHT.h>

// WiFi credentials
char ssid[] = "HUAWEI Mate 10 Pro";
char pass[] = "112233abc";

BlynkTimer timer;
WidgetRTC rtc;

// DHT11 sensor setup
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float temperature;

void clockDisplay() {
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();

  // Send time and date to Blynk app
  Blynk.virtualWrite(V1, currentTime);
  Blynk.virtualWrite(V2, currentDate);
  Blynk.virtualWrite(V4, temperature);
}

void sendTemperature() {
  temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Blynk.virtualWrite(V4, temperature);
}

BLYNK_CONNECTED() {
  rtc.begin();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();  // Start DHT sensor
  setSyncInterval(10 * 60); // Sync interval (10 minutes)

  timer.setInterval(1000L, clockDisplay);
  timer.setInterval(2000L, sendTemperature);
}

void loop() {
  Blynk.run();
  timer.run();

  float t = dht.readTemperature();
}

