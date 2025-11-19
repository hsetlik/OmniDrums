#pragma once

#include "../../OmniState.h"

class SampleCache : public ValueTree::Listener {
private:
  OmniState* const state;

public:
  SampleCache(OmniState* s);
  ~SampleCache();
};
