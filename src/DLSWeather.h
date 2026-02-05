#ifndef DLSWeather_h
#define DLSWeather_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

class DLSWeather {
public:
    DLSWeather(String stationId, String apiKey, float lat, float lon);

    void begin() {} // For compatibility

    void temperature(float val);
    void humidity(float val);
    void pressure(float val);
    void uvIndex(float val);
    void airQuality(int val);
    void windSpeed(float val);
    void windDirection(float val);
    void rainRate(float val);
    void rainDaily(float val);

    void battery(int val);
    void voltage(float val);
    void solar(float val);

    bool send(unsigned long timestamp);
    int getLastCode();

private:
    String _stationId;
    String _apiKey;
    float _lat;
    float _lon;

    float _temp;
    float _humi;
    float _press;
    float _uv;
    float _windSpd;
    float _windDir;
    float _rainRate;
    float _rainDaily;
    int _airQuality;

    int _battery;
    float _voltage;
    float _solar;
    
    int _lastCode;
};

#endif
