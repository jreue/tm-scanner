#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

class TftController {
  public:
    TftController();
    void setup();

    void enableBacklight();
    void disableBacklight();
    void updateRemainingTime(int32_t hours, int32_t minutes, int32_t seconds);
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

    void renderScanResultItem(const char* name, const char* status, uint16_t statusColor,
                              int32_t y);
};