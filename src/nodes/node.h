//
//  node.h
//  rayz
//
//  Created by Иван Дмитриевский on 13/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#ifndef __rayz__node__
#define __rayz__node__

#include <memory>
#include <vector>
#include "../geometries/geometry.h"

class node {
public:
  void add_child(std::unique_ptr<node> &&child);

  virtual geometry::ray_path intersect(ray ray) const = 0;
  virtual num_t get_color(vec3 point) const = 0;
  virtual vec3 get_normal(vec3 point) const = 0;

  std::vector<const node *> children() const;

  void set_lcs(const mat4 &lcs);
  mat4 get_lcs() const;

protected:
  mat4 _lcs;
  std::vector<std::unique_ptr<node> > _children;
};

#endif /* defined(__rayz__node__) */
