//
//  camera.cpp
//  rayz
//
//  Created by Иван Дмитриевский on 14/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#include "camera.h"
#include "glm/gtx/rotate_vector.hpp"

camera::camera(dvec3 pos, double fovx, double fovy, dvec3 lookat)
    : _eye(pos),
      _lookat(lookat),
      _up({ 0, 0, 1 }),
      _w(glm::normalize(_eye - _lookat)),
      _u(glm::normalize(glm::cross(_up, _w))),
      _v(glm::cross(_w, _u)),
      _dist(1),
      _fovx(fovx),
      _fovy(fovy) {}

camera::camera(dvec3 pos, double fovx, double fovy, double heading,
               double pitch, double roll)
    : camera(pos, fovx, fovy, pos + dvec3(0, 1, 0)) {

  double dist = glm::distance(_lookat, _eye);

  _w = glm::rotate(_w, -glm::radians(heading), _v);
  _u = glm::rotate(_u, -glm::radians(heading), _v);

  _w = glm::rotate(_w, -glm::radians(pitch), _u);
  _v = glm::rotate(_v, -glm::radians(pitch), _u);

  _u = glm::rotate(_u, -glm::radians(roll), _w); //
  _v = glm::rotate(_v, -glm::radians(roll), _w); //

  _lookat = _eye - _w * dist;
}

png::image<png::rgb_pixel> camera::render(const scene& scene,
                                          unsigned long resX,
                                          unsigned long resY) {
  png::image<png::rgb_pixel> img(resX, resY);
  std::vector<std::vector<double> > pxs(resY, std::vector<double>(resX, 0));

  double pxw = 2 * _dist * glm::tan(glm::radians(_fovx / 2)) / resX;
  double pxh = 2 * _dist * glm::tan(glm::radians(_fovy / 2)) / resY;

  double max_intensity = 0;

  double x = -(resX * pxw / 2);
  double y = -(resY * pxh / 2);

  for (long i = 0; i < resY; ++i) {
    for (long j = 0; j < resX; ++j) {
      for (auto& node : scene.nodes()) {
        ray ray;
        ray.origin = _eye;
        ray.dir = glm::normalize(x * _u + y * _v - _dist * _w);
        dvec3 ip;
        if (node->intersect(ray, &ip)) {
          max_intensity = glm::max(max_intensity, glm::distance(_eye, ip));
          pxs[i][j] = glm::distance(_eye, ip);
        }
      }
      x += pxw;
    }
    x = -(resX * pxw / 2);
    y += pxh;
  }

  for (long i = 0; i < resY; ++i) {
    for (long j = 0; j < resX; ++j) {
      double intensity = pxs[i][j] / max_intensity;
      unsigned char in = intensity * 255;
      img.set_pixel(j, i, { in, in, in });
    }
  }

  return img;
}