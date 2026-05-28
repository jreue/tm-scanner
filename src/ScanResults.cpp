#include "ScanResults.h"

ScanResult ScanResults::results[] = {
    {101, "Plasma Field Selector", false, false},        // tm-device-switch
    {102, "Planar Drift Matrix", false, false},          // tm-device-orientation
    {103, "Chrono Stabilizer Cube", false, false},       // tm-device-cube
    {104, "Dimensional Phase Modulator", false, false},  // tm-device-rings
    {105, "Uranium Fuel Cell", false, false},            // tm-device-temperature
    {106, "Polar Neutrino Disruptor", false, false},     // tm-device-keycard
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

void ScanResults::updateState(int index, bool connected, bool calibrated) {
  if (index >= 0 && (size_t)index < resultCount) {
    results[index].connected = connected;
    results[index].calibrated = calibrated;
  }
}
