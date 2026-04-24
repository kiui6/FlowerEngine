#pragma once

#include <Math/Bounds.h>

template <typename T>
class Quadtree {
    
public:
    void Insert(const T& obj, const AABB& bounds);
    void Query(const AABB& region, std::vector<T>& out);
};