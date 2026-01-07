#include "EspNowHelper.h"
#include "TftController.h"
#include "hardware_config.h"

TftController tftController;

bool scanInProgress = false;
uint8_t hubAddress[] = HUB_MAC_ADDRESS;
EspNowHelper espNowHelper;

void setup() {
  Serial.begin(115200);
  Serial2.begin(UART_BAUD_RATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  pinMode(SCAN_DEVICE_PIN, INPUT_PULLUP);
  pinMode(SCAN_ENVIRONMENT_PIN, INPUT_PULLUP);
  pinMode(EXTRA_PIN, INPUT_PULLUP);

  tftController.setup();

  espNowHelper.begin(hubAddress, DEVICE_ID);
  espNowHelper.sendConnected();
}

void loop() {
  int buttonState = digitalRead(SCAN_DEVICE_PIN);

  if (digitalRead(SCAN_ENVIRONMENT_PIN) == LOW) {
    Serial.println("ENV PIN");
  }

  if (digitalRead(EXTRA_PIN) == LOW) {
    Serial.println("EXTRA PIN");
  }

  if (buttonState == LOW && !scanInProgress) {
    tftController.showDeviceScanScreen();

    delay(500);

    Serial.println("START_SCAN");
    // send a message indicating scan started
    scanInProgress = true;
  }

  if (scanInProgress & Serial2.available()) {
    String msg = Serial2.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(msg);

    // Parse CSV string: "hours,minutes,seconds"
    int32_t hours = 0, minutes = 0, seconds = 0;
    int parsed = sscanf(msg.c_str(), "%d,%d,%d", &hours, &minutes, &seconds);
    if (parsed == 3) {
      tftController.updateRemainingTime(hours, minutes, seconds);
    } else {
      Serial.println("Error: Invalid time format received.");
    }
  }
}