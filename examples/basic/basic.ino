#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DLSWeather.h>

// --- CONFIGURATION ---
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* stationId = "STATION_ID";
const char* apiKey = "YOUR_API_KEY";
const float lat = 0.0;
const float lon = 0.0;

// --- OBJECTS ---
DLSWeather dls(stationId, apiKey, lat, lon);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    
    // Initialize NTP and Library
    timeClient.begin();
    dls.begin();
}

void loop() {
    // Update NTP Time
    timeClient.update();
    
    // 1. Environment Data
    dls.temperature(24.5);   // Celsius
    dls.humidity(60.2);      // Percentage
    dls.pressure(1013.25);   // hPa
    dls.uvIndex(3.5);        // UV Index
    dls.airQuality(45);      // IAQ Index (e.g., from BME680)
    
    // 2. Wind Data
    dls.windSpeed(12.5);     // m/s or km/h (depending on station config)
    dls.windDirection(270);  // Degrees (0-360)
    
    // 3. Rain Data
    dls.rainRate(2.5);       // mm/h
    dls.rainDaily(10.2);     // mm
    
    // 4. Send Data
    // We send data using Unix Timestamp (Epoch)
    unsigned long now = timeClient.getEpochTime();
    
    if (dls.send(now)) {
        Serial.println("DLS Weather: Data sent successfully!");
    } else {
        Serial.println("DLS Weather: Failed to send data.");
    }
    
    // Wait for 30 minutes (1800000 ms)
    delay(1800000);
}
