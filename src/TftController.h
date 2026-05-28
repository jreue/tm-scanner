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

    void showBootScreen(int cycles);
    void showMenuScreen();
    void showDeviceScanScreen();
    void showScanEnvironmentScreen(bool success);

    bool isDeviceScanActive() const;
    bool isMenuActive() const;
    void setCurrentDate(uint8_t month, uint8_t day, uint16_t year);

    void animateRadar();

  private:
    enum class Screen { NONE, MENU, DEVICE_SCAN, ENV_SCAN };
    Screen currentScreen = Screen::NONE;

    uint8_t _dateMonth = 0;
    uint8_t _dateDay = 0;
    uint16_t _dateYear = 0;

    TFT_eSPI tft;

    void renderScanEnvironmentBackground();
    void renderScanTerminal();
    void renderScanResults(bool success);

    void renderOuterBorder();
    void renderDividerLines();
    void renderSignalContainer();
    void renderHeaderText();

    void renderAllScanItems();
    void renderHomeFooter();
    void renderScanResultItem(const ScanResult& result, size_t index);
    void renderItemStateIndicator(size_t index, bool connected, bool calibrated);
    void renderItemName(size_t index, const String& name);
    void renderItemStatusLabel(size_t index, bool connected, bool calibrated);

    int32_t calculateItemY(size_t index);
};