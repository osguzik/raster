#pragma once

#include "render/raster/vertex.hpp"

int clipNearPlane(const ClipVertex &v0, const ClipVertex &v1, const ClipVertex &v2, float near, ClipVertex resultTriangles[2][3]);

