#pragma once

#include "render/rasterize/vertex.hpp"

int clipNearPlane(const ClipVertex &v0, const ClipVertex &v1,
                  const ClipVertex &v2, float near,
                  ClipVertex clippedTriangles[2][3]);
