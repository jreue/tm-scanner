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

    void animateRadar();

  private:
    enum class Screen { NONE, MENU, DEVICE_SCAN, ENV_SCAN };
    Screen currentScreen = Screen::NONE;

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