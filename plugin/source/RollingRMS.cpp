//===================================================
#include "OmniDrums/Audio/Shared/RollingRMS.h"

void RollingRMS::tick(float value) {
  // 1. subtract the oldest sample
  runningSum -= ringBuf.front();
  // 2. overwrite the oldest sample
  ringBuf.push(std::fabs(value));
  // 3. add the newest sample
  runningSum += ringBuf.front();
}

float RollingRMS::getCurrentLevel() const {
  return runningSum / (float)RMS_LENGTH;
}
