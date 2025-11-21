#include "OmniDrums/GUI/Shared/Util.h"

namespace GraphicsUtil {
frect_t scaledFor(const frect_t& rect, float xScale, float yScale) {
  return {rect.getX() * xScale, rect.getY() * yScale, rect.getWidth() * xScale,
          rect.getHeight() * yScale};
}
}  // namespace GraphicsUtil

//===================================================
