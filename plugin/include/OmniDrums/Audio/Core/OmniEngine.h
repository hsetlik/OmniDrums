#pragma once
#include "../../OmniState.h"

class OmniEngine {
private:
  OmniState* const state;

public:
  OmniEngine(OmniState* s);
};
