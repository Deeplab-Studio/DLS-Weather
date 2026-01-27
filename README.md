# DLSWeather Library

Universal ESP32 weather API client for the **DLS Weather** platform. This library is designed to be lightweight, hardware-agnostic, and compatible with any internet-connected ESP32 board.

## Features

- Hardware-independent (use any sensor like BMP280, BME280, BME680, etc.)
- Dynamic JSON payload matching the latest DLS Weather API
- Easy to use setter methods for all weather metrics
- Works with any Wi-Fi connection method

## Installation

Add the following to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/Deeplab-Studio/DLSWeather
```

## Basic Usage

```cpp
#include <DLSWeather.h>

DLSWeather dls("STATION_ID", "API_KEY", 40.9, 29.1);

void setup() {
    dls.begin();
}

void loop() {
    dls.temperature(25.0);
    dls.humidity(60.0);
    dls.send(epochTime); // Send data to API
}
```

## License

MIT License
