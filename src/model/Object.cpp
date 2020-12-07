/**
  *  Author: Zhaobo Ding (me@dingzhaobo.net)
  */
#include "Object.h"

void Object::setMaterial(Material mat) {
    this->mat = mat;
}

void Object::setMapping(mapping *map) {
  bump_mapping = map;
}