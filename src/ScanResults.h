#pragma once

#include <Arduino.h>

struct ScanResult {
    uint8_t id;
    String name;
    bool connected;
    bool calibrated;
};

class ScanResults {
  public:
    static const ScanResult* getResults();
    static size_t getResultCount();
    static int getIndexById(uint8_t id);
    static void updateState(int index, bool connected, bool calibrated);

  private:
    static ScanResult results[];
    static const size_t resultCount;
};