#include "camera.hpp"

#include "math/mat.hpp"
#include "math/transform.hpp"

Mat4 viewMat(const Camera &camera) {
  return lookAt(camera.eye, camera.at, camera.up);
}

Mat4 projectionMat(const Camera &camera) {
  return perspective(camera.fovY, camera.ar, camera.near, camera.far);
}
