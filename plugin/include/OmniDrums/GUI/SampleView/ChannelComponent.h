#pragma once
#include "../../OmniState.h"
#include "../Shared/Color.h"
#include "../Shared/Fonts.h"

class OmniChannelComponent : public Component {
public:
  OmniChannelComponent(OmniState* s, int channelIdx);
};

