#include "render/clipping.hpp"

#include "render/rasterize/vertex.hpp"

namespace {

ClipVertex interpolateVertex(const ClipVertex &from, const ClipVertex &to,
                             float factor) {
  return ClipVertex{from.pos + (to.pos - from.pos) * factor,
                    from.worldPos + (to.worldPos - from.worldPos) * factor,
                    from.uv + (to.uv - from.uv) * factor,
                    from.normal + (to.normal - from.normal) * factor,
                    from.color + (to.color - from.color) * factor};
}

float intersectionFactor(const ClipVertex &from, const ClipVertex &to,
                         float near) {
  float fromDistance = from.pos.w - near;
  float toDistance = to.pos.w - near;
  return fromDistance / (fromDistance - toDistance);
}

} // namespace

int clipNearPlane(const ClipVertex &v0, const ClipVertex &v1,
                  const ClipVertex &v2, float near,
                  ClipVertex clippedTriangles[2][3]) {
  bool in0 = v0.pos.w >= near;
  bool in1 = v1.pos.w >= near;
  bool in2 = v2.pos.w >= near;
  int insideCount = in0 + in1 + in2;

  switch (insideCount) {
  case 0: {
    return 0;
  }

  case 1: {
    const ClipVertex *in;
    const ClipVertex *outA;
    const ClipVertex *outB;

    if (in0) {
      in = &v0;
      outA = &v1;
      outB = &v2;
    } else if (in1) {
      in = &v1;
      outA = &v2;
      outB = &v0;
    } else {
      in = &v2;
      outA = &v0;
      outB = &v1;
    }

    ClipVertex clipA =
        interpolateVertex(*in, *outA, intersectionFactor(*in, *outA, near));
    ClipVertex clipB =
        interpolateVertex(*in, *outB, intersectionFactor(*in, *outB, near));

    clippedTriangles[0][0] = *in;
    clippedTriangles[0][1] = clipA;
    clippedTriangles[0][2] = clipB;
    return 1;
  }

  case 2: {
    const ClipVertex *out;
    const ClipVertex *inA;
    const ClipVertex *inB;

    if (!in0) {
      out = &v0;
      inA = &v1;
      inB = &v2;
    } else if (!in1) {
      out = &v1;
      inA = &v2;
      inB = &v0;
    } else {
      out = &v2;
      inA = &v0;
      inB = &v1;
    }

    ClipVertex clipA =
        interpolateVertex(*inA, *out, intersectionFactor(*inA, *out, near));
    ClipVertex clipB =
        interpolateVertex(*inB, *out, intersectionFactor(*inB, *out, near));

    clippedTriangles[0][0] = *inA;
    clippedTriangles[0][1] = *inB;
    clippedTriangles[0][2] = clipA;
    clippedTriangles[1][0] = clipA;
    clippedTriangles[1][1] = *inB;
    clippedTriangles[1][2] = clipB;
    return 2;
  }

  case 3: {
    clippedTriangles[0][0] = v0;
    clippedTriangles[0][1] = v1;
    clippedTriangles[0][2] = v2;
    return 1;
  }

  default: {
    return 0;
  }
  }
}
