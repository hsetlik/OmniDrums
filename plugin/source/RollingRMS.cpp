//===================================================
#include "OmniDrums/Audio/Shared/RollingRMS.h"

void RollingRMS::tick(float value) {
  // 1. subtract the oldest sample
  runningSum -= ringBuf.front();
  // 2. add the newest value
  auto absVal = std::fabs(value);
  runningSum += absVal;
  // 3. update the ring buffer
  ringBuf.push(absVal);
}

float RollingRMS::getCurrentLevel() const {
  return runningSum / (float)RMS_LENGTH;
}
