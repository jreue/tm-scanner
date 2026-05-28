#include "ScanResults.h"

const ScanResult ScanResults::results[] = {
    {101, "Plasma Field Selector"},        // tm-device-switch
    {102, "Planar Drift Matrix"},          // tm-device-orientation
    {103, "Chrono Stabilizer Cube"},       // tm-device-cube
    {104, "Dimensional Phase Modulator"},  // tm-device-rings
    {105, "Uranium Fuel Cell"},            // tm-device-temperature
    {106, "Polar Neutrino Disruptor"},     // tm-device-keycard
};

const size_t ScanResults::resultCount = sizeof(ScanResults::results) / sizeof(ScanResult);

const ScanResult* ScanResults::getResults() {
  return results;
}

size_t ScanResults::getResultCount() {
  return resultCount;
}

int ScanResults::getIndexById(uint8_t id) {
  for (size_t i = 0; i < resultCount; ++i) {
    if (results[i].id == id) {
      return i;
    }
  }
  return -1;
}
