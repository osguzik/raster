#include "render/clipping.hpp"

#include "render/raster/vertex.hpp"

int clipNearPlane(const ClipVertex &v0, const ClipVertex &v1, const ClipVertex &v2, float near, ClipVertex resultTriangles[2][3]) {
  bool in0 = v0.pos.w >= near;
  bool in1 = v1.pos.w >= near;
  bool in2 = v2.pos.w >= near;
  int insideCount = in0 + in1 + in2;

  switch (insideCount) {
    case 0: {
      return 0;
    }

    case 1: {
      return 0; // todo
    }

    case 2: {
      return 0; // todo
    }

    case 3: {
      resultTriangles[0][0] = v0;
      resultTriangles[0][1] = v1;
      resultTriangles[0][2] = v2;
      return 1;
    }

    default: {
      return 0;
    }
  }
}

