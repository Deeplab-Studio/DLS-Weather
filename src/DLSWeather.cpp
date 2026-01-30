#include "DLSWeather.h"

DLSWeather::DLSWeather(String stationId, String apiKey, float lat, float lon) {
    _stationId = stationId;
    _apiKey = apiKey;
    _lat = lat;
    _lon = lon;

    // Default to NAN to identify uninitialized values
    _temp = NAN;
    _humi = NAN;
    _press = NAN;
    _uv = NAN;
    _windSpd = NAN;
    _windDir = NAN;
    _rainRate = NAN;
    _rainDaily = NAN;
    _airQuality = -1;
}

void DLSWeather::temperature(float val) { _temp = val; }
void DLSWeather::humidity(float val) { _humi = val; }
void DLSWeather::pressure(float val) { _press = val; }
void DLSWeather::uvIndex(float val) { _uv = val; }
void DLSWeather::airQuality(int val) { _airQuality = val; }
void DLSWeather::windSpeed(float val) { _windSpd = val; }
void DLSWeather::windDirection(float val) { _windDir = val; }
void DLSWeather::rainRate(float val) { _rainRate = val; }
void DLSWeather::rainDaily(float val) { _rainDaily = val; }

bool DLSWeather::send(unsigned long timestamp) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[DLS] Error: WiFi not connected");
        return false;
    }

    // Explicitly use WiFiClientSecure for compatibility check
    WiFiClientSecure client;
    client.setInsecure(); // Skip certificate validation
    client.setHandshakeTimeout(30); // Increase timeout for slow SSL
    
    HTTPClient http;
    http.begin(client, "https://wx-api.deeplabstudio.com/v1/ingest/weather");
    http.addHeader("Content-Type", "application/json");
    if (_apiKey.length() > 0) {
        http.addHeader("x-api-key", _apiKey);
    }

    JsonDocument doc;
    doc["stationId"] = _stationId;
    doc["timestamp"] = timestamp;

    JsonObject location = doc["location"].to<JsonObject>();
    location["lat"] = _lat;
    location["lon"] = _lon;

    // Environment
    JsonObject environment = doc["environment"].to<JsonObject>();
    if (!isnan(_temp)) environment["temperature"] = _temp;
    if (!isnan(_humi)) environment["humidity"] = _humi;
    if (!isnan(_press)) environment["pressure"] = _press;
    if (!isnan(_uv)) environment["uv_index"] = _uv;
    if (_airQuality != -1) environment["air_quality"] = _airQuality;

    // Wind
    if (!isnan(_windSpd) || !isnan(_windDir)) {
        JsonObject wind = doc["wind"].to<JsonObject>();
        if (!isnan(_windSpd)) wind["speed"] = _windSpd;
        if (!isnan(_windDir)) wind["direction"] = _windDir;
    }

    // Rain
    if (!isnan(_rainRate) || !isnan(_rainDaily)) {
        JsonObject rain = doc["rain"].to<JsonObject>();
        if (!isnan(_rainRate)) rain["rate"] = _rainRate;
        if (!isnan(_rainDaily)) rain["daily"] = _rainDaily;
    }

    String jsonOutput;
    serializeJson(doc, jsonOutput);

    Serial.println("[DLS] Sending payload:");
    Serial.println(jsonOutput);

    int httpResponseCode = http.POST(jsonOutput);
    _lastCode = httpResponseCode;

    bool success = false;
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.print("[DLS] HTTP Code: ");
        Serial.println(httpResponseCode);
        Serial.println("[DLS] Response: " + response);
        success = (httpResponseCode >= 200 && httpResponseCode < 300);
    } else {
        Serial.print("[DLS] HTTP Error: ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    
    http.end();
    return success;
}

int DLSWeather::getLastCode() {
    return _lastCode;
}
