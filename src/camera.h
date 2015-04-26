//
//  camera.h
//  rayz
//
//  Created by Иван Дмитриевский on 14/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __rayz__camera__
#define __rayz__camera__

#include "png++/png.hpp"
#include "scene.h"

class scene;

class camera {
public:
  camera(vec3 pos, num_t fov_x, num_t fov_y, vec3 lookat);
  camera(vec3 pos, num_t fov_x, num_t fov_y, num_t heading, num_t pitch,
         num_t roll);

  png::image<png::rgba_pixel_16> render(const scene& scene, long resX,
                                        long resY);

private:
  vec3 _eye;
  vec3 _lookat;
  vec3 _up;

  vec3 _w;
  vec3 _u;
  vec3 _v;

  num_t _dist;
  num_t _fovx, _fovy;
};

#endif /* defined(__rayz__camera__) */
