#include "ScanResults.h"

const ScanResult ScanResults::results[] = {{"Temporal Drift Sensor", "Offline", 0xFDA0},
                                           {"Chrono-Stabilizer", "Offline", 0xFDA0},
                                           {"Quantum Flux Regulator", "Offline", 0xFDA0},
                                           {"Dimensional Phase Modulator", "Offline", 0xFDA0},
                                           {"Plasma Containment Field", "Offline", 0xFDA0},
                                           {"Neutrino Emission Matrix", "Offline", 0xFDA0},
                                           {"Environment Scanner", "Enabled", 0x07E0},
                                           {"Uranium Core", "Critical", 0xF800}};

const size_t ScanResults::resultCount = sizeof(ScanResults::results) / sizeof(ScanResult);

const ScanResult* ScanResults::getResults() {
  return results;
}

size_t ScanResults::getResultCount() {
  return resultCount;
}
