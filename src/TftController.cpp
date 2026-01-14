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
  showBootScreen(2);
  showMainScreen();
}

void TftController::enableBacklight() {
  digitalWrite(TFT_BL, HIGH);
}

void TftController::disableBacklight() {
  digitalWrite(TFT_BL, LOW);
}

void TftController::showBootScreen(int cycles) {
  tft.fillScreen(COLOR_BACKGROUND);
  String base = "BOOTING";
  int x = 200, y = 150;
  for (int i = 0; i <= cycles; ++i) {
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
  renderAllScanItems();
  // Reset text datum to left baseline
  tft.setTextDatum(TL_DATUM);
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
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SHIELD DIAGNOSTICS UNIT", 240, 31);
  tft.setTextDatum(TL_DATUM);
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

void TftController::renderAllScanItems() {
  const ScanResult* results = ScanResults::getResults();
  size_t count = ScanResults::getResultCount();
  for (size_t i = 0; i < count; ++i) {
    renderScanResultItem(results[i], i);
  }
}

void TftController::renderScanResultItem(const ScanResult& result, size_t index) {
  int32_t y = calculateItemY(index);

  renderItemStateIndicator(index, false, false);
  renderItemName(index, result.name);
  renderItemStatusLabel(index, false, false);
}

void TftController::renderItemStateIndicator(size_t index, bool connected, bool calibrated) {
  int32_t y = calculateItemY(index);

  if (!connected) {
    tft.drawBitmap(33, y, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  } else if (connected && !calibrated) {
    tft.drawBitmap(33, y, image_choice_bullet_on_bits, 15, 16, TFT_YELLOW);
  } else {
    tft.drawBitmap(33, y, image_choice_bullet_on_bits, 15, 16, TFT_GREEN);
  }
}

void TftController::updateItemStateIndicator(size_t index, bool connected, bool calibrated) {
  int32_t y = calculateItemY(index);

  // Clear the area where the bitmap is drawn
  tft.fillRect(33, y, 15, 16, COLOR_BACKGROUND);

  // Redraw with new state
  renderItemStateIndicator(index, connected, calibrated);
}

void TftController::updateItemStatusLabel(size_t index, bool connected, bool calibrated) {
  int32_t y = calculateItemY(index);

  // Clear the area where the status label is drawn
  tft.fillRect(300, y - 3, 155, 23, COLOR_BACKGROUND);

  // Redraw with new state
  renderItemStatusLabel(index, connected, calibrated);
}

void TftController::renderItemName(size_t index, const String& name) {
  int32_t y = calculateItemY(index);

  tft.setTextDatum(L_BASELINE);
  tft.setTextColor(0xE9F);
  tft.setFreeFont(&FreeSerifBold9pt7b);
  tft.drawString(name, 55, y + 13);
}

void TftController::renderItemStatusLabel(size_t index, bool connected, bool calibrated) {
  int32_t y = calculateItemY(index);

  String status;
  uint16_t statusColor;

  if (!connected) {
    status = "Offline";
    statusColor = 0xF800;  // Red
  } else if (connected && !calibrated) {
    status = "Calibrate";
    statusColor = TFT_YELLOW;
  } else {
    status = "Engaged";
    statusColor = TFT_GREEN;
  }

  tft.setTextDatum(R_BASELINE);
  tft.setTextColor(statusColor);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString(status, 445, y + 13);
}

int32_t TftController::calculateItemY(size_t index) {
  int32_t startY = 75;
  int32_t lineHeight = 25;
  return startY + index * lineHeight;
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