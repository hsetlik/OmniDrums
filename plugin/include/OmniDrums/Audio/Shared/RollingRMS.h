#pragma once
#include "../../OmniState.h"

#define RMS_LENGTH 256

class RingBuffer {
private:
  const size_t bufLength;
  std::unique_ptr<float[]> buf;
  size_t head = 0;

public:
  RingBuffer(size_t length = RMS_LENGTH)
      : bufLength(length), buf(new float[length]) {
    std::fill(&buf[0], &buf[0] + bufLength, 0.0f);
  }
  void push(float val) {
    buf[head] = val;
    head = (head + 1) % bufLength;
  }
  size_t size() const { return bufLength; }
  // read the newest value
  float front() const { return buf[head]; }
  // read the oldest value
  float back() const { return buf[(head + 1) % bufLength]; }
};

//------------------------------------------------------------

class RollingRMS {
private:
  RingBuffer ringBuf;
  float runningSum = 0.0f;

public:
  RollingRMS() = default;
  void tick(float value);
  float getCurrentLevel() const;
};
