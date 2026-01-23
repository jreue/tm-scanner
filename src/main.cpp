#include "EspNowHelper.h"
#include "ScanResults.h"
#include "TftController.h"
#include "hardware_config.h"

TftController tftController;

bool scanInProgress = false;
uint8_t hubAddress[] = HUB_MAC_ADDRESS;
EspNowHelper espNowHelper;

// Button Handlers
void handleScanDeviceButtonPress();
void handleScanEnvironmentButtonPress();
void handleExtraButtonPress();

void handleShieldModuleMessage(const ShieldModuleMessage& msg);

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
  espNowHelper.registerModuleMessageHandler(handleShieldModuleMessage);
  espNowHelper.sendScannerConnected();
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

void handleShieldModuleMessage(const ShieldModuleMessage& msg) {
  Serial.println("Handling Shield Module Message:");
  Serial.printf("  Shield Module ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);
  Serial.printf("  Is Calibrated: %s\n", msg.isCalibrated ? "Yes" : "No");

  // Get the index for this shield module id
  int index = ScanResults::getIndexById(msg.deviceId);
  if (index >= 0) {
    bool connected = true;  // Since we received a message, the shield module is connected
    tftController.updateItemStateIndicator(index, connected, msg.isCalibrated);
    tftController.updateItemStatusLabel(index, connected, msg.isCalibrated);
  } else {
    Serial.printf("  Warning: Unknown shield module ID %d\n", msg.deviceId);
  }
}