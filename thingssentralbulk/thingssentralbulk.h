#ifndef thingssentralbulk_h
#define thingssentralbulk_h

#include <ArduinoJson.h>
#include <time.h>

// Platform detection
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#else
#error "Unsupported platform. This library only supports ESP8266 and ESP32"
#endif

class ThingsSentralBulk {
public:
  ThingsSentralBulk(const char* serverURL, const char* nodeID)
    : _serverURL(serverURL), _nodeID(nodeID) {
    resetBuffer();
  }

  // Static NTP configuration
  static void configureNTP(int timezoneOffset = 0,
                           const char* ntpServer1 = "pool.ntp.org",
                           const char* ntpServer2 = "time.nist.gov") {
    configTime(timezoneOffset * 3600, 0, ntpServer1, ntpServer2);
    while (!time(nullptr)) {
      delay(500);
    }
  }

  // Add data point with automatic timestamp
  void addData(float value) {
    _addDataInternal(value, _getCurrentTime());
  }

  // Add data point with custom timestamp
  void addData(float value, const char* timestamp) {
    _addDataInternal(value, timestamp);
  }

  // Send accumulated data
  bool send() {
    if (_dataCount == 0) return false;

    JsonDocument doc;
    doc["nodeid"] = _nodeID;
    doc["data"] = _dataValues;
    doc["datetime"] = _timestamps;

    String jsonStr;
    serializeJson(doc, jsonStr);

    String url = _serverURL + "/SendArray?DataToSend=" + _urlEncode(jsonStr);

    // testing
    if (0) {
      Serial.println(url);
      Serial.println(_serverURL + "/SendArray?DataToSend=" + jsonStr);
      resetBuffer();
      return 1;
    }

    HTTPClient http;
#ifdef ESP32
    http.begin(url);
#else
    WiFiClient client;
    http.begin(client, url);
#endif

    int httpCode = http.GET();
    http.end();

    resetBuffer();
    return httpCode == HTTP_CODE_OK;
  }

  void resetBuffer() {
    _dataValues = "";
    _timestamps = "";
    _dataCount = 0;
  }

  int count() {
    return _dataCount;
  }

private:
  String _serverURL;
  String _nodeID;
  String _dataValues;
  String _timestamps;
  int _dataCount = 0;

  void _addDataInternal(float value, const String& timestamp) {
    if (_dataCount > 0) {
      _dataValues += '|';
      _timestamps += '|';
    }

    _dataValues += String(value);
    _timestamps += timestamp;
    _dataCount++;
  }

  String _urlEncode(const String& str) {
    String encoded;
    for (unsigned int i = 0; i < str.length(); i++) {
      char c = str[i];
      if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
        encoded += c;
      } else if (c == ' ') {
        encoded += "%20";
      } else {
        char buf[4];
        sprintf(buf, "%%%02X", c);
        encoded += buf;
      }
    }
    return encoded;
  }

  String _getCurrentTime() {
    time_t now = time(nullptr);
    if (now < 24 * 3600) return "00:00";

    struct tm* timeinfo = localtime(&now);
    char timeStr[6];
    strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
    return String(timeStr);
  }
};

#endif