#include "TftController.h"

#include "ScanResults.h"
#include "images.h"
#include "scan.h"

#define COLOR_BACKGROUND 0x83
#define COLOR_OUTER_BORDER 0xE9F
#define COLOR_TITLE_TEXT 0x87FF

#define COLOR_LIGHT_BLUE 0xE9F
#define COLOR_DARK_BLUE 0x948
#define COLOR_SUCCESS_GREEN TFT_GREEN
#define COLOR_FAILURE_RED TFT_RED

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
  showBootScreen(8);
  showMenuScreen();
}

void TftController::enableBacklight() {
  digitalWrite(TFT_BL, HIGH);
}

void TftController::disableBacklight() {
  digitalWrite(TFT_BL, LOW);
}

bool TftController::isDeviceScanActive() const {
  return currentScreen == Screen::DEVICE_SCAN;
}

bool TftController::isMenuActive() const {
  return currentScreen == Screen::MENU;
}

void TftController::setCurrentDate(uint8_t month, uint8_t day, uint16_t year) {
  _dateMonth = month;
  _dateDay = day;
  _dateYear = year;
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
  currentScreen = Screen::MENU;
  tft.fillRectVGradient(0, 0, 480, 320, TFT_DARKGREY, COLOR_BACKGROUND);

  renderOuterBorder();
  renderDividerLines();
  renderHeaderText();
  renderSignalContainer();

  // Scan Time Machine Option
  tft.drawRoundRect(51, 94, 380, 40, 5, COLOR_LIGHT_BLUE);
  tft.drawEllipse(79, 113, 14, 14, COLOR_LIGHT_BLUE);
  tft.setTextColor(COLOR_LIGHT_BLUE);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("SCAN THE TIME MACHINE", 111, 105);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("A", 71, 104);

  // Scan Environment Option
  tft.drawRoundRect(51, 150, 380, 40, 5, COLOR_LIGHT_BLUE);
  tft.drawEllipse(79, 169, 14, 14, COLOR_LIGHT_BLUE);
  tft.setTextColor(COLOR_LIGHT_BLUE);
  tft.setFreeFont(&FreeMonoBold12pt7b);
  tft.drawString("SCAN THE ENVIRONMENT", 111, 161);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.drawString("B", 71, 160);

  if (_dateMonth != 0) {
    char dateBuf[26];
    snprintf(dateBuf, sizeof(dateBuf), "Current Date: %02d/%02d/%04d", _dateMonth, _dateDay,
             _dateYear);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeMonoBold9pt7b);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(dateBuf, 240, 284);
    tft.setTextDatum(TL_DATUM);
  }
}

void TftController::showDeviceScanScreen() {
  currentScreen = Screen::DEVICE_SCAN;
  tft.fillRectVGradient(0, 0, 480, 320, TFT_DARKGREY, COLOR_BACKGROUND);

  renderOuterBorder();
  renderDividerLines();
  renderHeaderText();
  renderSignalContainer();

  renderAllScanItems();
  // Reset text datum to left baseline
  tft.setTextDatum(TL_DATUM);

  renderHomeFooter();
}

void TftController::showScanEnvironmentScreen(bool success) {
  currentScreen = Screen::ENV_SCAN;
  renderScanEnvironmentBackground();
  renderScanTerminal();
  tft.fillScreen(COLOR_BACKGROUND);
  renderScanEnvironmentBackground();
  renderScanResults(success);

  renderHomeFooter();
}

void TftController::renderScanEnvironmentBackground() {
  tft.fillScreen(COLOR_BACKGROUND);
  tft.setSwapBytes(true);
  tft.pushImage(0, 32, 480, 250, scan_image_bits);
}

void TftController::renderScanTerminal() {
  // Terminal Border
  tft.fillRoundRect(243, 70, 225, 186, 7, COLOR_LIGHT_BLUE);
  // Terminal Fill
  tft.fillRoundRect(245, 71, 222, 183, 7, COLOR_DARK_BLUE);

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
  // Info Border
  tft.fillRoundRect(243, 132, 225, 57, 7, COLOR_LIGHT_BLUE);
  // Info Fill
  tft.fillRoundRect(245, 133, 222, 55, 7, COLOR_DARK_BLUE);
  if (success) {
    // Success Icon
    tft.drawBitmap(255, 140, image_crosshairs_bits, 15, 16, COLOR_SUCCESS_GREEN);
    // Success Text
    tft.setTextSize(1);
    tft.setTextColor(COLOR_SUCCESS_GREEN);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.drawString("STRONG MODULE", 281, 141);
    tft.drawString("SIGNAL DETECTED!", 277, 164);
  } else {
    // Error Icon
    tft.drawBitmap(255, 140, image_operation_warning_bits, 16, 16, COLOR_FAILURE_RED);
    // Error Text
    tft.setTextSize(1);
    tft.setTextColor(COLOR_FAILURE_RED);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.drawString("NO MODULE", 305, 141);
    tft.drawString("SIGNAL DETECTED", 277, 164);
  }
}
void TftController::renderHomeFooter() {
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeMonoBold9pt7b);
  tft.drawString("RETURN TO HOME SCREEN", 143, 286);

  tft.drawEllipse(122, 293, 10, 10, COLOR_LIGHT_BLUE);

  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString("C", 115, 286);
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
  tft.setTextSize(1);
  tft.setTextColor(COLOR_LIGHT_BLUE);
  tft.setFreeFont(&FreeSansBold12pt7b);

  tft.setTextDatum(TC_DATUM);
  tft.drawString("SHIELD DIAGNOSTICS UNIT", 240, 31);
  tft.setTextDatum(TL_DATUM);
}

void TftController::renderSignalContainer() {
  tft.fillRoundRect(187, 1, 106, 17, 3, COLOR_OUTER_BORDER);
  // wifi_not_connected
  tft.drawBitmap(197, 2, image_wifi_not_connected_bits, 19, 16, TFT_BLACK);
  // network_not_connected
  tft.drawBitmap(233, 2, image_network_not_connected_bits, 15, 16, TFT_BLACK);
  // battery_full
  tft.drawBitmap(262, 2, image_battery_full_bits, 24, 16, TFT_BLACK);
}

void TftController::renderAllScanItems() {
  const ScanResult* results = ScanResults::getResults();
  size_t count = ScanResults::getResultCount();
  for (size_t i = 0; i < count; ++i) {
    renderScanResultItem(results[i], i);
  }
}

void TftController::renderScanResultItem(const ScanResult& result, size_t index) {
  renderItemStateIndicator(index, result.connected, result.calibrated);
  renderItemName(index, result.name);
  renderItemStatusLabel(index, result.connected, result.calibrated);
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

void TftController::renderItemName(size_t index, const String& name) {
  int32_t y = calculateItemY(index);

  tft.setTextColor(COLOR_LIGHT_BLUE);
  tft.setFreeFont(&FreeSerifBold9pt7b);
  tft.setTextDatum(L_BASELINE);
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
