#pragma once

#include "Vectors.h"

struct Vertex2D {
    Float2 x, y;
    Float2 u, v;
};

struct Vertex3D {
    Float3 x, y, z;
    Float3 u, v;
};

struct Primitives {
    constexpr static Vertex2D TRIANGLE[3] = {{{0, 0}, {0, 0}}, 
                                             {{0, 1}, {0, 1}},
                                             {{1, 0}, {0, 1}}};

    constexpr static Vertex2D SQUARE[6] = {{{0, 0}, {0, 0}}, 
                                           {{0, 1}, {0, 1}},
                                           {{1, 0}, {1, 0}},
                                           {{0, 1}, {0, 1}},
                                           {{1, 1}, {1, 1}},
                                           {{1, 0}, {0, 1}}};
};