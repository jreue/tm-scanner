#include "ScanResults.h"

const ScanResult ScanResults::results[] = {
    {101, "Temporal Drift Sensor"},    {102, "Chrono-Stabilizer"},
    {103, "Quantum Flux Regulator"},   {104, "Dimensional Phase Modulator"},
    {105, "Plasma Containment Field"}, {106, "Neutrino Emission Matrix"},
    {107, "Environment Scanner"},      {108, "Uranium Core"}};

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
