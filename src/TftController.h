#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

struct ScanResult;

class TftController {
  public:
    TftController();
    void setup();

    void enableBacklight();
    void disableBacklight();
    void updateRemainingTime(int32_t hours, int32_t minutes, int32_t seconds);
    void updateItemStateIndicator(size_t index, bool connected, bool calibrated);
    void showDeviceScanScreen();

  private:
    TFT_eSPI tft;

    void showBootScreen();
    void showMainScreen();

    void renderOuterBorder();
    void renderDividerLines();
    void renderSignalContainer();
    void renderHeaderText();
    void renderRemainingTimeLabel();
    void renderRemainingTimeSuffix();

    void renderAllScanItems();
    void renderScanResultItem(const ScanResult& result, size_t index);
    void renderItemStateIndicator(size_t index, bool connected, bool calibrated);
    void renderItemName(size_t index, const String& name);
    void renderItemStatus(size_t index, const String& status, uint16_t statusColor);
    int32_t calculateItemY(size_t index);
};