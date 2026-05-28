#include <Button.h>
#include <shared_hardware_config.h>

#include "EspNowHelper.h"
#include "ScanResults.h"
#include "TftController.h"
#include "hardware_config.h"

struct ValidDate {
    uint8_t month;
    uint8_t day;
    uint16_t year;
};

const ValidDate VALID_DATES[] = {
    {12, 25, 1975},
    {2, 1, 1977},
};
const int VALID_DATES_COUNT = sizeof(VALID_DATES) / sizeof(ValidDate);

TftController tftController;

uint8_t hubAddress[] = HUB_MAC_ADDRESS;
EspNowHelper espNowHelper;

DateMessage currentDate = {month : 0, day : 0, year : 0};

void setupButtons();

// Button Handlers
void handleScanDeviceButtonPress(void* button_handle, void* usr_data);
void handleScanEnvironmentButtonPress(void* button_handle, void* usr_data);
void handleExtraButtonPress(void* button_handle, void* usr_data);

void handleShieldModuleMessage(const ShieldModuleMessage& msg);
void handleDateMessage(const DateMessage& msg);
bool isValidDate();

void setup() {
  Serial.begin(115200);
  Serial.println("Scanner Starting...");
  Serial.printf("Scanner Device ID: %d\n", DEVICE_ID);

  setupButtons();

  tftController.setup();

  espNowHelper.begin(DEVICE_ID);
  espNowHelper.addPeer(hubAddress);
  espNowHelper.registerModuleMessageHandler(handleShieldModuleMessage);
  espNowHelper.registerDateMessageHandler(handleDateMessage);
  espNowHelper.sendScannerConnected(hubAddress);
}

void loop() {
  // tftController.animateRadar();
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
  tftController.showScanEnvironmentScreen(isValidDate());
}

void handleExtraButtonPress(void* button_handle, void* usr_data) {
  Serial.println("Pressed Extra Button");
  tftController.showMenuScreen();
}

bool isValidDate() {
  for (int i = 0; i < VALID_DATES_COUNT; ++i) {
    if (VALID_DATES[i].month == currentDate.month && VALID_DATES[i].day == currentDate.day &&
        VALID_DATES[i].year == currentDate.year) {
      return true;
    }
  }
  return false;
}

void handleDateMessage(const DateMessage& msg) {
  Serial.println("Handling Date Message:");
  Serial.printf("  Date: %02d/%02d/%04d\n", msg.month, msg.day, msg.year);
  currentDate = msg;
  tftController.setCurrentDate(msg.month, msg.day, msg.year);
  if (tftController.isMenuActive()) {
    tftController.showMenuScreen();
  }
}

void handleShieldModuleMessage(const ShieldModuleMessage& msg) {
  Serial.println("Handling Shield Module Message:");
  Serial.printf("  Shield Module ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);
  Serial.printf("  Is Calibrated: %s\n", msg.isCalibrated ? "Yes" : "No");

  // Get the index for this shield module id
  int index = ScanResults::getIndexById(msg.deviceId);
  if (index >= 0) {
    ScanResults::updateState(index, true, msg.isCalibrated);
    if (tftController.isDeviceScanActive()) {
      tftController.showDeviceScanScreen();
    }
  } else {
    Serial.printf("  Warning: Unknown shield module ID %d\n", msg.deviceId);
  }
}