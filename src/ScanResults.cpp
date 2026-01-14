#include "ScanResults.h"

const ScanResult ScanResults::results[] = {{101, "Temporal Drift Sensor", "Offline", 0xF800},
                                           {102, "Chrono-Stabilizer", "Offline", 0xF800},
                                           {103, "Quantum Flux Regulator", "Offline", 0xF800},
                                           {104, "Dimensional Phase Modulator", "Offline", 0xF800},
                                           {105, "Plasma Containment Field", "Offline", 0xF800},
                                           {106, "Neutrino Emission Matrix", "Offline", 0xF800},
                                           {107, "Environment Scanner", "Offline", 0xF800},
                                           {108, "Uranium Core", "Offline", 0xF800}};

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
