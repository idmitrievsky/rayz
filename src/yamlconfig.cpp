//
//  yamlconfig.cpp
//  rayz
//
//  Created by Иван Дмитриевский on 14/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#include "yamlconfig.h"

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

std::unique_ptr<node> parse_node(YAML::Node node_config) {
  YAML::Node ymlnode;
  std::unique_ptr<node> parsed_node = nullptr;
  std::unique_ptr<geometry> geom = nullptr;

  if ((ymlnode = node_config[1]["triangle"])) {
    geom = parse_triangle(ymlnode);
  } else if ((ymlnode = node_config[1]["cylinder"])) {
    geom = parse_cylinder(ymlnode);
  } else if ((ymlnode = node_config[1]["torus"])) {
    geom = parse_torus(ymlnode);
  } else if ((ymlnode = node_config[1]["sphere"])) {
    geom = parse_sphere(ymlnode);
  };

  if (geom) {
    parsed_node = make_unique<node>(std::move(geom));
  }

  if (parsed_node) {
    for (unsigned long i = 2; i < node_config.size() && node_config[i]["node"];
         ++i) {
      auto child = std::move(parse_node(node_config[i]["node"]));
      if (child) {
        parsed_node->add_child(std::move(child));
      }
    }
  }

  return parsed_node;
}

std::unique_ptr<triangle> parse_triangle(YAML::Node triangle_config) {
  std::unique_ptr<triangle> parsed_node = nullptr;
  std::array<dvec3, 3> args;
  std::array<std::string, 3> keys({ "p0", "p1", "p2" });

  for (unsigned long i = 0; i < args.size(); ++i) {
    auto point_config = triangle_config[keys[i]];
    if (!point_config) {
      return nullptr;
    } else {
      args[i].x = point_config["x"].as<double>();
      args[i].y = point_config["y"].as<double>();
      args[i].z = point_config["z"].as<double>();
    }
  }

  return make_unique<triangle>(args);
}

std::unique_ptr<cylinder> parse_cylinder(YAML::Node cylinder_config) {
  if (!cylinder_config["radius"] || !cylinder_config["height"]) {
    return nullptr;
  }

  return make_unique<cylinder>(cylinder_config["radius"].as<double>(),
                               cylinder_config["height"].as<double>());
}

std::unique_ptr<torus> parse_torus(YAML::Node torus_config) {
  if (!torus_config["radius"] || !torus_config["tube_radius"]) {
    return nullptr;
  }

  return make_unique<torus>(torus_config["radius"].as<double>(),
                            torus_config["tube_radius"].as<double>());
}

std::unique_ptr<sphere> parse_sphere(YAML::Node sphere_config) {
  if (!sphere_config["radius"]) {
    return nullptr;
  }

  return make_unique<sphere>(dvec3{ 0, 50, 0 },
                             sphere_config["radius"].as<double>());
}

std::unique_ptr<camera> parse_camera(YAML::Node camera_config) {
  dvec3 pos, orient;
  double fovx = 0, fovy = 0;

  if (!camera_config) {
    return nullptr;
  }

  auto position_config = camera_config["position"];
  if (!position_config) {
    return nullptr;
  } else {
    pos.x = position_config["x"].as<double>();
    pos.y = position_config["y"].as<double>();
    pos.z = position_config["z"].as<double>();
  }

  auto orient_config = camera_config["orientation"];
  if (!orient_config) {
    return nullptr;
  } else {
    orient.x = orient_config["h"].as<double>();
    orient.y = orient_config["p"].as<double>();
    orient.z = orient_config["r"].as<double>();
  }

  auto fx_config = camera_config["fov_x"];
  if (!fx_config) {
    return nullptr;
  } else {
    fovx = fx_config.as<double>();
  }

  auto fy_config = camera_config["fov_y"];
  if (!fy_config) {
    return nullptr;
  } else {
    fovy = fy_config.as<double>();
  }

  return make_unique<camera>(pos, fovx, fovy, orient.x, orient.y, orient.z);
}