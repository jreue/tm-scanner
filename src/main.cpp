#include <Button.h>
#include <shared_hardware_config.h>

#include "EspNowHelper.h"
#include "ScanResults.h"
#include "TftController.h"
#include "hardware_config.h"

TftController tftController;

uint8_t hubAddress[] = HUB_MAC_ADDRESS;
EspNowHelper espNowHelper;

void setupButtons();

// Button Handlers
void handleScanDeviceButtonPress(void* button_handle, void* usr_data);
void handleScanEnvironmentButtonPress(void* button_handle, void* usr_data);
void handleExtraButtonPress(void* button_handle, void* usr_data);

void handleShieldModuleMessage(const ShieldModuleMessage& msg);

void setup() {
  Serial.begin(115200);
  Serial.println("Scanner Starting...");
  Serial.printf("Scanner Device ID: %d\n", DEVICE_ID);

  setupButtons();

  tftController.setup();

  espNowHelper.begin(DEVICE_ID);
  espNowHelper.addPeer(hubAddress);
  espNowHelper.registerModuleMessageHandler(handleShieldModuleMessage);
  espNowHelper.sendScannerConnected(hubAddress);
}

void loop() {
  delay(200);
}

void setupButtons() {
  Serial.printf("Scan Device PIN: %d\n", SCAN_DEVICE_PIN);
  Button* scanDeviceButton = new Button(SCAN_DEVICE_PIN, false);
  scanDeviceButton->attachSingleClickEventCb(&handleScanDeviceButtonPress, NULL);

  Serial.printf("Scan Environment PIN: %d\n", SCAN_ENVIRONMENT_PIN);
  Button* scanEnvironmentButton = new Button(SCAN_ENVIRONMENT_PIN, false);
  scanEnvironmentButton->attachSingleClickEventCb(&handleScanEnvironmentButtonPress, NULL);

  Serial.printf("Extra PIN: %d\n", EXTRA_PIN);
  Button* extraButton = new Button(EXTRA_PIN, false);
  extraButton->attachSingleClickEventCb(&handleExtraButtonPress, NULL);
}

void handleScanDeviceButtonPress(void* button_handle, void* usr_data) {
  Serial.println("Pressed Scan Device Button");

  tftController.showDeviceScanScreen();
}

void handleScanEnvironmentButtonPress(void* button_handle, void* usr_data) {
  Serial.println("Pressed Scan Environment Button");
}

void handleExtraButtonPress(void* button_handle, void* usr_data) {
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