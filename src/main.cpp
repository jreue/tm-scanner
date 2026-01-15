#include "EspNowHelper.h"
#include "ScanResults.h"
#include "TftController.h"
#include "hardware_config.h"

TftController tftController;

bool scanInProgress = false;
uint8_t hubAddress[] = HUB_MAC_ADDRESS;
EspNowHelper espNowHelper;

// Generic Message Handler
void handleESPNowDataReceived(const uint8_t* mac, const uint8_t* incomingDataRaw, int len);

// Button Handlers
void handleScanDeviceButtonPress();
void handleScanEnvironmentButtonPress();
void handleExtraButtonPress();

void handleDeviceMessage(const DeviceMessage& msg);

void setup() {
  Serial.begin(115200);
  Serial.println("Scanner Starting...");
  Serial.printf("Scanner Device ID: %d\n", DEVICE_ID);

  Serial.printf("Scan Device PIN: %d\n", SCAN_DEVICE_PIN);
  Serial.printf("Scan Environment PIN: %d\n", SCAN_ENVIRONMENT_PIN);
  Serial.printf("Extra PIN: %d\n", EXTRA_PIN);
  pinMode(SCAN_DEVICE_PIN, INPUT_PULLUP);
  pinMode(SCAN_ENVIRONMENT_PIN, INPUT_PULLUP);
  pinMode(EXTRA_PIN, INPUT_PULLUP);

  tftController.setup();

  espNowHelper.begin(hubAddress, DEVICE_ID);
  espNowHelper.sendConnected();

  esp_now_register_recv_cb(handleESPNowDataReceived);
}

void loop() {
  if (digitalRead(SCAN_DEVICE_PIN) == LOW && !scanInProgress) {
    scanInProgress = true;
    handleScanDeviceButtonPress();
    delay(500);  // Debounce delay
  }

  if (digitalRead(SCAN_ENVIRONMENT_PIN) == LOW) {
    handleScanEnvironmentButtonPress();
  }

  if (digitalRead(EXTRA_PIN) == LOW) {
    handleExtraButtonPress();
  }

  delay(200);
}

void handleScanEnvironmentButtonPress() {
  Serial.println("Pressed Scan Environment Button");
}

void handleScanDeviceButtonPress() {
  Serial.println("Pressed Scan Device Button");

  tftController.showDeviceScanScreen();
}

void handleExtraButtonPress() {
  Serial.println("Pressed Extra Button");
}

void handleESPNowDataReceived(const uint8_t* mac, const uint8_t* incomingDataRaw, int len) {
  // Read message header to determine type
  EspNowHeader header;
  memcpy(&header, incomingDataRaw, sizeof(EspNowHeader));

  Serial.println("--- ESP-NOW Data Received ---");
  Serial.print("MAC Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5)
      Serial.print(":");
  }
  Serial.println();

  Serial.printf("ESP ID: %d\n", header.deviceId);
  Serial.printf("Device Type: %d\n", header.deviceType);
  Serial.printf("Message Type: %d\n", header.messageType);

  if (header.deviceType == DEVICE_TYPE_MODULE) {
    DeviceMessage deviceMsg;
    memcpy(&deviceMsg, incomingDataRaw, sizeof(DeviceMessage));
    handleDeviceMessage(deviceMsg);
  } else {
    Serial.println("Unknown Message Type Received");
  }

  Serial.println("-----------------------------");
}

void handleDeviceMessage(const DeviceMessage& msg) {
  Serial.println("Handling Device Message:");
  Serial.printf("  Device ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);
  Serial.printf("  Is Calibrated: %s\n", msg.isCalibrated ? "Yes" : "No");

  // Get the index for this device id
  int index = ScanResults::getIndexById(msg.deviceId);
  if (index >= 0) {
    bool connected = true;  // Since we received a message, the device is connected
    tftController.updateItemStateIndicator(index, connected, msg.isCalibrated);
    tftController.updateItemStatusLabel(index, connected, msg.isCalibrated);
  } else {
    Serial.printf("  Warning: Unknown device ID %d\n", msg.deviceId);
  }
}