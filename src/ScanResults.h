#pragma once

#include <Arduino.h>

struct ScanResult {
    String name;
    String status;
    uint16_t statusColor;
};

class ScanResults {
  public:
    static const ScanResult* getResults();
    static size_t getResultCount();

  private:
    static const ScanResult results[];
    static const size_t resultCount;
};