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
    void showScanResultsScreen();

  private:
    TFT_eSPI tft;

    void showBootScreen();
    void showMainScreen();

    void displayRemainingTimeLabel();

    // void testDisplay();
    // void printPinConfig();
    // const char* pinToString(int32_t pin);
    // void drawPinConfig(int32_t idx, const char* pinName, int32_t pin, uint32_t color);
};