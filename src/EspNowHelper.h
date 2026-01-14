#pragma once

#include <Arduino.h>
#include <MessageStructs.h>
#include <WiFi.h>
#include <esp_now.h>

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
