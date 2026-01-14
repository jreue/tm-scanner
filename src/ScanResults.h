#pragma once

#include <Arduino.h>

struct ScanResult {
    uint8_t id;
    String name;
};

class ScanResults {
  public:
    static const ScanResult* getResults();
    static size_t getResultCount();
    static int getIndexById(uint8_t id);

  private:
    static const ScanResult results[];
    static const size_t resultCount;
};