//
//  sphere.h
//  rayz
//
//  Created by Иван Дмитриевский on 12/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __rayz__sphere__
#define __rayz__sphere__

#include "geometry.h"

class sphere : public geometry {
public:
  sphere(fvec3 pos, float rad);
  virtual bool intersect(ray ray, fvec3 *intersection_point) const override;
  virtual float get_color(fvec3 point) const override;
  virtual fvec3 get_normal(fvec3 point) const override;

private:
  fvec3 _pos;
  float _rad;
};

#endif /* defined(__rayz__sphere__) */
