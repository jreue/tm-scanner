#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Message types
#define MSG_TYPE_CONNECT 0
#define MSG_TYPE_STATUS 1
#define MSG_TYPE_DISCONNECT 2
#define MSG_TYPE_DATE_UPDATE 3
#define MSG_TYPE_SCANNER_CONNECTED 4

// Common header for all messages
struct EspNowHeader {
    uint8_t id;
    uint8_t messageType;
};

typedef struct ScannerMessage {
    uint8_t id;
    uint8_t messageType;  // MSG_TYPE_CONNECT, MSG_TYPE_STATUS, MSG_TYPE_DISCONNECT
} ScannerMessage;

// Device message (connection/calibration status)
struct DeviceMessage {
    uint8_t id;
    uint8_t messageType;
    bool isCalibrated;
};

class EspNowHelper {
  public:
    EspNowHelper();

    void begin(uint8_t* hubMacAddress, uint8_t deviceId);
    void sendConnected();

  private:
    uint8_t* receiverAddress;
    ScannerMessage message;
    DeviceMessage deviceMessage;

    void sendCalibrationStatus();
    static void handleESPNowDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);
};
