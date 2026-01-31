#include "TftController.h"

#include "ScanResults.h"
#include "images.h"
#include "scan.h"

#define COLOR_BACKGROUND 0x83
#define COLOR_OUTER_BORDER 0xE9F
#define COLOR_TITLE_TEXT 0x87FF

// --- Global scan message pool ---
const char* SCAN_MESSAGES[] = {
    "Initializing quantum phase array...", "Calibrating temporal sensors...",
    "Multi-spectrum analysis engaged...",  "Decrypting telemetry stream...",
    "Syncing with orbital relay...",       "Analyzing subspace harmonics...",
    "Compiling resonance data...",         "Mapping photon topology...",
    "Verifying crypto handshake...",       "Integrating neural scan matrix...",
    "Activating photon amplifiers...",     "Scanning for module signatures...",
    "Establishing secure link...",         "Extracting signal harmonics...",
    "Detecting molecular resonance...",    "Aligning quantum field sensors...",
    "Configuring neural scan matrix...",   "Analyzing electron topology...",
    "Synchronizing time phase arrays...",  "Decrypting encrypted telemetry...",
    "Compiling phase resonance data...",   "Mapping subspace frequencies...",
    "Engaging crypto handshake...",        "Activating orbital relay sync...",
    "Analyzing photon emission data..."};
const int SCAN_MESSAGE_POOL_SIZE = sizeof(SCAN_MESSAGES) / sizeof(SCAN_MESSAGES[0]);

TftController::TftController() : tft() {
}

void TftController::setup() {
  tft.init();
  tft.setRotation(1);  // 1 = landscape, 0 = portrait

  enableBacklight();
  showBootScreen(3);
  showMenuScreen();

  // tft.fillScreen(COLOR_BACKGROUND);
  // showScanEnvironmentScreen(true);
  // delay(3000);
  // showScanEnvironmentScreen(false);
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

void TftController::showMenuScreen() {
  tft.fillRectVGradient(0, 0, 480, 320, TFT_DARKGREY, COLOR_BACKGROUND);

  renderOuterBorder();
  renderDividerLines();
  renderHeaderText();
  renderSignalContainer();

  tft.setFreeFont(&FreeSerifBold9pt7b);

  // Select Action Text
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Select an Action:", 175, 70);

  tft.setTextColor(0xE9F);
  tft.setFreeFont(&FreeMonoBold12pt7b);

  // Scan Time Machine Option
  tft.drawRoundRect(51, 94, 380, 40, 5, 0xE9F);
  tft.drawEllipse(79, 113, 14, 14, 0xE9F);
  tft.drawString("SCAN THE TIME MACHINE", 111, 105);

  // Scan Environment Option
  tft.drawRoundRect(51, 150, 380, 40, 5, 0xE9F);
  tft.drawEllipse(79, 169, 14, 14, 0xE9F);
  tft.drawString("SCAN THE ENVIRONMENT", 111, 161);

  // Return Home Option
  tft.drawRoundRect(51, 208, 380, 40, 5, 0xE9F);
  tft.drawEllipse(79, 227, 14, 14, 0xE9F);
  tft.drawString("RETURN TO HOME SCREEN", 111, 219);

  tft.setTextColor(0xFFFF);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("A", 71, 104);
  tft.drawString("B", 71, 160);
  tft.drawString("C", 71, 218);
}

void TftController::showDeviceScanScreen() {
  tft.fillRectVGradient(0, 0, 480, 320, TFT_DARKGREY, COLOR_BACKGROUND);

  renderOuterBorder();
  renderDividerLines();
  renderHeaderText();
  renderSignalContainer();
  // renderRemainingTimeLabel();
  // renderRemainingTimeSuffix();
  renderAllScanItems();
  // Reset text datum to left baseline
  tft.setTextDatum(TL_DATUM);
}

void TftController::showScanEnvironmentScreen(bool success) {
  renderScanEnvironmentBackground();
  renderScanTerminal();
  tft.fillScreen(COLOR_BACKGROUND);
  renderScanEnvironmentBackground();
  renderScanResults(success);
}

void TftController::renderScanEnvironmentBackground() {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.setSwapBytes(true);
  tft.pushImage(0, 32, 480, 250, scan_image_bits);
}

void TftController::renderScanTerminal() {
  // Draw terminal-like rounded rectangle
  tft.fillRoundRect(243, 70, 225, 186, 7, 0x473F);
  tft.fillRoundRect(245, 71, 222, 183, 7, 0x948);

  // Wave Icon
  tft.drawBitmap(440, 80, image_music_sound_wave_bits, 17, 16, TFT_WHITE);

  // Prepare scan sequence: always start and end with fixed messages
  const char* startMsg = "Connecting...";
  const char* endMsg = "Scan complete.";
  const int scanSequenceLen = 9;
  const int yStart = 86;
  const int xStart = 255;

  // Select 7 random messages from pool
  int usedIndices[scanSequenceLen - 2] = {0};
  for (int i = 0; i < scanSequenceLen - 2; ++i) {
    int idx;
    bool unique;
    do {
      unique = true;
      idx = rand() % SCAN_MESSAGE_POOL_SIZE;
      // Ensure uniqueness
      for (int j = 0; j < i; ++j) {
        if (usedIndices[j] == idx) {
          unique = false;
          break;
        }
      }
    } while (!unique);
    usedIndices[i] = idx;
  }

  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);  // Smallest built-in font (GLCD)
  tft.setTextSize(1);
  tft.setFreeFont(NULL);  // Use built-in font, not FreeFont

  // Print start message
  tft.drawString(startMsg, xStart, yStart);
  delay(1000 + (rand() % 1001));

  // Print 7 random messages
  for (int i = 0; i < scanSequenceLen - 2; ++i) {
    tft.drawString(SCAN_MESSAGES[usedIndices[i]], xStart, yStart + (i + 1) * 18);
    delay(1000 + (rand() % 1001));
  }

  // Print end message
  tft.drawString(endMsg, xStart, yStart + (scanSequenceLen - 1) * 18);
  delay(1000 + (rand() % 1001));
}

void TftController::renderScanResults(bool success) {
  // Info Background
  tft.fillRoundRect(243, 132, 225, 57, 7, 0x473F);
  // Border
  tft.fillRoundRect(245, 133, 222, 55, 7, 0x948);
  if (success) {
    // Success Icon
    tft.drawBitmap(255, 140, image_crosshairs_bits, 15, 16, 0x7E0);
    // Success Text
    tft.setTextColor(0x7E0);
    tft.setTextSize(1);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.drawString("STRONG MODULE", 281, 141);
    tft.drawString("SIGNAL FOUND!", 276, 164);
  } else {
    // Error Icon
    tft.drawBitmap(255, 140, image_operation_warning_bits, 16, 16, 0xF800);
    // Error Text
    tft.setTextColor(0xF800);
    tft.setTextSize(1);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.drawString("NO MODULE", 305, 141);
    tft.drawString("SIGNAL DETECTED", 277, 164);
  }
}

void TftController::animateRadar() {
  static float angle = 0.0f;
  static int prev_x = -1, prev_y = -1;
  const int origin_x = 141;
  const int origin_y = 160;
  const int length = 100;
  const uint16_t color = TFT_WHITE;
  const uint16_t bg = COLOR_BACKGROUND;

  // Erase previous line if it exists
  if (prev_x != -1 && prev_y != -1) {
    tft.drawLine(origin_x, origin_y, prev_x, prev_y, bg);
  }

  // Calculate new end point
  float rad = angle * 3.14159265f / 180.0f;
  int x2 = origin_x + (int)(length * cos(rad));
  int y2 = origin_y + (int)(length * sin(rad));

  // Draw new line
  tft.drawLine(origin_x, origin_y, x2, y2, color);

  // Store for next erase
  prev_x = x2;
  prev_y = y2;

  // Increment angle
  angle += 3.0f;  // Adjust speed as needed
  if (angle >= 360.0f)
    angle -= 360.0f;

  // Add a small delay for visible animation (optional)
  delay(20);
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
