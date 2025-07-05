# ThingsSentralBulk Library

![ESP32 and ESP8266 Compatible](https://img.shields.io/badge/ESP32%20%26%20ESP8266-Compatible-brightgreen)
![MIT License](https://img.shields.io/badge/License-MIT-blue)

A lightweight, efficient library for sending bulk sensor data to ThingsSentral IoT servers from ESP32 and ESP8266 devices. Optimized for sending batched sensor readings with automatic timestamping and efficient memory management.

## Key Features

- 📦 **Bulk Data Transmission**: Send multiple sensor readings in a single API call
- ⏱️ **Automatic Timestamping**: Built-in NTP synchronization for accurate timekeeping
- 🌐 **Cross-Platform**: Works seamlessly with both ESP32 and ESP8266
- 🧠 **Dynamic Memory Handling**: Automatically adjusts to any number of data points
- 🔧 **Simple API**: Easy-to-use methods for adding and sending data
- 🔒 **URL Encoding**: Automatic encoding of special characters in payloads
- 📡 **Multi-Node Support**: Create multiple instances for different sensors

## Installation

### Using Arduino IDE (Recommended) (fake)
1. Download the [latest release](https://github.com/yourusername/thingssentralbulk/releases)
2. In Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the downloaded ZIP file

### Manual Installation (fake)
1. Clone or download this repository
2. Copy the `thingssentralbulk.h` file to your Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\ThingsSentralBulk\`
   - Linux/Mac: `~/Arduino/libraries/ThingsSentralBulk/`

## Dependencies
- [ArduinoJSON](https://arduinojson.org/) (v6.19.4 or higher)
- [HTTPClient](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient) (Bundled with ESP boards)
- [WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) (Bundled with ESP boards)
- [Time](https://github.com/PaulStoffregen/Time) (Bundled with Arduino)

## Getting Started

### Basic Usage Example
```cpp
#include "thingssentralbulk.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverURL = "http://myIot.com";

// Create a sender instance
ThingsSentralBulk sensor(serverURL, "LivingRoom");

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  // Configure NTP (GMT+8)
  ThingsSentralBulk::configureNTP(8);
}

void loop() {
  static unsigned long lastSample = 0;
  
  // Sample every second
  if (millis() - lastSample >= 1000) {
    float temperature = readTemperature(); // Your sensor function
    sensor.addData(temperature); // Automatic timestamp
    lastSample = millis();
  }
  
  // Send every minute
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 60000) {
    if (sensor.send()) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Send failed - will retry next cycle");
    }
    lastSend = millis();
  }
}
```

## API Reference

### Class: `ThingsSentralBulk`

#### Constructor
```cpp
ThingsSentralBulk(const char* serverURL, const char* nodeID)
```
- `serverURL`: Base URL of your ThingsSentral server (e.g., "http://myIot.com.e")
- `nodeID`: Unique identifier for your sensor node

#### Methods

| Method | Description |
|--------|-------------|
| `static void configureNTP(int timezoneOffset = 0, const char* ntpServer1 = "pool.ntp.org", const char* ntpServer2 = "time.nist.gov")` | Configures NTP time synchronization. Call once in setup. |
| `void addData(float value)` | Adds a data point with automatic timestamp |
| `void addData(float value, const char* timestamp)` | Adds a data point with custom timestamp |
| `bool send()` | Sends accumulated data to server. Returns true on success. |
| `void resetBuffer()` | Clears all buffered data |
| `int count()` | Returns number of data points currently buffered |

### Advanced Usage

#### Multiple Sensors Example
```cpp
ThingsSentralBulk tempSensor(serverURL, "Temperature");
ThingsSentralBulk humiditySensor(serverURL, "Humidity");

void loop() {
  // ... sampling code ...
  
  tempSensor.addData(readTemperature());
  humiditySensor.addData(readHumidity());
  
  // Send both every 5 minutes
  if (millis() - lastSend >= 300000) {
    tempSensor.send();
    humiditySensor.send();
    lastSend = millis();
  }
}
```

#### Custom Timestamps
```cpp
// Add data with specific timestamp
sensor.addData(23.5, "14:30");
```

#### Manual Buffer Reset
```cpp
if (!sensor.send()) {
  Serial.println("Send failed - resetting buffer");
  sensor.resetBuffer();
}
```

## Memory Management

The library dynamically allocates memory for JSON payloads based on actual data size. Here's the formula used:

```cpp
const size_t capacity = _nodeID.length() + 
                       _dataValues.length() + 
                       _timestamps.length() + 100;
```

- **ESP8266**: Recommended max 60 samples (fake)
- **ESP32**: Can handle 500+ samples (tested with 1000 samples) (fake)

## Best Practices (fake)

1. **Sample Rate**: Match your hardware capabilities
2. **Send Interval**: Balance between data freshness and power consumption
3. **Error Handling**: Implement retry logic for failed transmissions
4. **Time Zones**: Configure correct timezone offset in `configureNTP()`
5. **WiFi Management**: Ensure stable connection before sending
6. **Buffer Monitoring**: Use `count()` to verify expected data accumulation

## Troubleshooting (fake)
 
| Issue | Solution |
|-------|----------|
| Send fails with HTTP error 400 | Verify server URL format and JSON structure |
| Time shows "00:00" | Check NTP configuration and internet connection |
| Memory allocation fails | Reduce number of samples per send |
| Data not reaching server | Enable debug mode in HTTPClient library |
| WiFi disconnects frequently | Implement WiFi reconnect logic in your sketch |

## Contributing (fake)

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a pull request

## License

This library is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

## Support

For support, feature requests, or bug reports:
- [Open an Issue](https://github.com/yourusername/thingssentralbulk/issues)
- Join our [Community Forum](https://forum.thingssentral.com)
- Email: support@thingssentral.com

---

**ThingsSentralBulk** - Efficient bulk data transmission for your IoT projects 🚀
