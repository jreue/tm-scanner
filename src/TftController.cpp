#include "TftController.h"

#include "ScanResults.h"
#include "images.h"

#define COLOR_BACKGROUND 0x83
#define COLOR_OUTER_BORDER 0xE9F
#define COLOR_TITLE_TEXT 0x87FF

TftController::TftController() : tft() {
}

void TftController::setup() {
  tft.init();
  tft.setRotation(1);  // 1 = landscape, 0 = portrait

  enableBacklight();
  showBootScreen();
  delay(2000);
  showMainScreen();
}

void TftController::enableBacklight() {
  digitalWrite(TFT_BL, HIGH);
}

void TftController::disableBacklight() {
  digitalWrite(TFT_BL, LOW);
}

void TftController::showBootScreen() {
  tft.fillScreen(COLOR_BACKGROUND);
  String base = "BOOTING";
  int x = 200, y = 150;
  for (int i = 0; i <= 6; ++i) {
    // Clear the area where the text will be drawn (adjust width/height as needed)
    tft.fillRect(x, y - 16, 120, 24, COLOR_BACKGROUND);
    String dots = "";
    for (int d = 0; d < i; ++d) dots += ".";
    tft.drawString(base + dots, x, y);
    delay(750);
  }
}

void TftController::showMainScreen() {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.drawString("Press button to scan", 200, 150);
}

void TftController::showDeviceScanScreen() {
  tft.fillScreen(COLOR_BACKGROUND);

  renderOuterBorder();
  renderDividerLines();
  renderHeaderText();
  renderSignalContainer();
  renderRemainingTimeLabel();
  renderRemainingTimeSuffix();

  int32_t startY = 75;
  int32_t lineHeight = 25;
  const ScanResult* results = ScanResults::getResults();
  size_t count = ScanResults::getResultCount();
  for (size_t i = 0; i < count; ++i) {
    renderScanResultItem(results[i].name.c_str(), results[i].status.c_str(), results[i].statusColor,
                         startY + i * lineHeight);
  }
}

void TftController::renderOuterBorder() {
  tft.drawRoundRect(11, 10, 460, 300, 18, COLOR_OUTER_BORDER);
}

void TftController::renderDividerLines() {
  // HeaderUnderline
  tft.drawLine(30, 56, 449, 56, COLOR_OUTER_BORDER);
  // BottomUnderline
  tft.drawLine(30, 278, 449, 278, COLOR_OUTER_BORDER);
}

void TftController::renderHeaderText() {
  tft.setTextColor(0xE9F);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("TEMPORAL DIAGNOSTICS UNIT", 53, 31);
}

void TftController::renderSignalContainer() {
  tft.fillRoundRect(187, 1, 106, 17, 3, COLOR_OUTER_BORDER);
  // wifi_not_connected
  tft.drawBitmap(197, 2, image_wifi_not_connected_bits, 19, 16, 0x0);
  // network_not_connected
  tft.drawBitmap(233, 2, image_network_not_connected_bits, 15, 16, 0x0);
  // battery_full
  tft.drawBitmap(262, 2, image_battery_full_bits, 24, 16, 0x0);
}

void TftController::renderRemainingTimeLabel() {
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("THERMAL MELTDOWN IN:", 33, 286);
}

void TftController::renderRemainingTimeSuffix() {
  // HH MM SS Suffix
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("H", 313, 283);
  tft.drawString("M", 368, 283);
  tft.drawString("S", 425, 283);
}

void TftController::renderScanResultItem(const char* name, const char* status, uint16_t statusColor,
                                         int32_t y) {
  // Bullet
  tft.drawBitmap(33, y, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);

  // Name
  tft.setTextColor(0xE9F);
  tft.setFreeFont(&FreeSerifBold9pt7b);
  tft.drawString(name, 55, y);

  // Status
  tft.setTextColor(statusColor);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawRightString(status, 445, y - 2, 1);
}

void TftController::updateRemainingTime(int32_t hours, int32_t minutes, int32_t seconds) {
  tft.fillRect(282, 285, 28, 15, COLOR_BACKGROUND);  // Clear Hours
  tft.fillRect(337, 285, 28, 15, COLOR_BACKGROUND);  // Clear Minutes
  tft.fillRect(394, 285, 28, 15, COLOR_BACKGROUND);  // Clear Seconds

  tft.setTextColor(0xEF7D);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  char buf[3];
  sprintf(buf, "%02d", hours);
  tft.drawRightString(buf, 310, 283, 1);
  sprintf(buf, "%02d", minutes);
  tft.drawRightString(buf, 365, 283, 1);
  sprintf(buf, "%02d", seconds);
  tft.drawRightString(buf, 422, 283, 1);
}