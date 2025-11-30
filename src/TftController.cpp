#include "TftController.h"

#define COLOR_BACKGROUND 0x83
#define COLOR_OUTER_BORDER 0xE9F
#define COLOR_TITLE_TEXT 0x87FF

static const unsigned char PROGMEM image_battery_full_bits[] = {
    0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x10, 0x00, 0x01, 0x16, 0xdb, 0x6d, 0x76, 0xdb, 0x6d, 0x86,
    0xdb, 0x6d, 0x86, 0xdb, 0x6d, 0x86, 0xdb, 0x6d, 0x86, 0xdb, 0x6d, 0x86, 0xdb, 0x6d, 0x76, 0xdb,
    0x6d, 0x16, 0xdb, 0x6d, 0x10, 0x00, 0x01, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const unsigned char PROGMEM image_network_not_connected_bits[] = {
    0x82, 0x0e, 0x44, 0x0a, 0x28, 0x0a, 0x10, 0x0a, 0x28, 0xea, 0x44, 0xaa, 0x82, 0xaa, 0x00, 0xaa,
    0x0e, 0xaa, 0x0a, 0xaa, 0x0a, 0xaa, 0x0a, 0xaa, 0xea, 0xaa, 0xaa, 0xaa, 0xee, 0xee, 0x00, 0x00};

static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {
    0x21, 0xf0, 0x00, 0x16, 0x0c, 0x00, 0x08, 0x03, 0x00, 0x25, 0xf0, 0x80, 0x42, 0x0c, 0x40, 0x89,
    0x02, 0x20, 0x10, 0xa1, 0x00, 0x23, 0x58, 0x80, 0x04, 0x24, 0x00, 0x08, 0x52, 0x00, 0x01, 0xa8,
    0x00, 0x02, 0x04, 0x00, 0x00, 0x42, 0x00, 0x00, 0xa1, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00};

static const unsigned char PROGMEM image_choice_bullet_on_bits[] = {
    0x07, 0xc0, 0x1c, 0x70, 0x33, 0x98, 0x6f, 0xec, 0x5f, 0xf4, 0xdf, 0xf6, 0xbf, 0xfa, 0xbf, 0xfa,
    0xbf, 0xfa, 0xdf, 0xf6, 0x5f, 0xf4, 0x6f, 0xec, 0x33, 0x98, 0x1c, 0x70, 0x07, 0xc0, 0x00, 0x00};

static const unsigned char PROGMEM image_choice_bullet_off_bits[] = {
    0x07, 0xc0, 0x1c, 0x70, 0x30, 0x18, 0x60, 0x0c, 0x40, 0x04, 0xc0, 0x06, 0x80, 0x02, 0x80, 0x02,
    0x80, 0x02, 0xc0, 0x06, 0x40, 0x04, 0x60, 0x0c, 0x30, 0x18, 0x1c, 0x70, 0x07, 0xc0, 0x00, 0x00};

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

void TftController::showScanResultsScreen() {
  tft.fillScreen(COLOR_BACKGROUND);

  // OuterBorder
  tft.drawRoundRect(11, 10, 460, 300, 18, 0xE9F);

  // HeaderText
  tft.setTextColor(0xE9F);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("TEMPORAL DIAGNOSTICS UNIT", 53, 31);

  // signalContainer
  tft.fillRoundRect(187, 1, 106, 17, 3, 0xE9F);
  // wifi_not_connected
  tft.drawBitmap(197, 2, image_wifi_not_connected_bits, 19, 16, 0x0);
  // network_not_connected
  tft.drawBitmap(233, 2, image_network_not_connected_bits, 15, 16, 0x0);
  // battery_full
  tft.drawBitmap(262, 2, image_battery_full_bits, 24, 16, 0x0);

  // ThermalMeltdownLabel
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("THERMAL MELTDOWN IN:", 33, 286);

  // HH MM SS Suffix
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("H", 313, 283);
  tft.drawString("M", 368, 283);
  tft.drawString("S", 425, 283);

  // HeaderUnderline
  tft.drawLine(30, 56, 449, 56, 0xE9F);
  // BottomUnderline
  tft.drawLine(30, 278, 449, 278, 0xE9F);

  // STATUS ICONS
  tft.drawBitmap(33, 72, image_choice_bullet_on_bits, 15, 16, TFT_RED);
  tft.drawBitmap(33, 92, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  tft.drawBitmap(33, 112, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  tft.drawBitmap(33, 132, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  tft.drawBitmap(33, 152, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  tft.drawBitmap(33, 172, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);
  tft.drawBitmap(33, 192, image_choice_bullet_off_bits, 15, 16, TFT_WHITE);

  // STATUS LABELS
  tft.setTextColor(0xE9F);
  tft.setFreeFont(&FreeSerifBold9pt7b);
  tft.drawString("Uranium Core", 55, 72);
  tft.drawString("Temporal Drift Sensor", 55, 92);
  tft.drawString("Chrono-Stabilizer", 55, 112);
  tft.drawString("Quantum Flux Regulator", 55, 132);
  tft.drawString("Dimensional Phase Modulator", 55, 152);
  tft.drawString("Plasma Containment Field", 55, 172);
  tft.drawString("Neutrino Emission Matrix", 55, 192);

  // STATUS VALUES
  tft.setTextColor(TFT_RED);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawRightString("Critical", 445, 70, 1);
  tft.setTextColor(TFT_ORANGE);
  tft.drawRightString("Offline", 445, 90, 1);
  tft.drawRightString("Offline", 445, 110, 1);
  tft.drawRightString("Offline", 445, 130, 1);
  tft.drawRightString("Offline", 445, 150, 1);
  tft.drawRightString("Offline", 445, 170, 1);
  tft.drawRightString("Offline", 445, 190, 1);
}

void TftController::displayRemainingTimeLabel() {
  tft.setTextColor(COLOR_TITLE_TEXT, COLOR_BACKGROUND);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("THERMAL MELTDOWN IN:", 10, 200);
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