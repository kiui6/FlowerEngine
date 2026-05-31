#pragma once

#include "Vectors.h"

struct Vertex2D {
    Float2 pos;
    Float2 uv;
};

struct Vertex3D {
    Float3 pos;
    Float2 uv;
};

struct Primitives {
    constexpr static Vertex2D TRIANGLE[3] = {{{0, 0}, {0, 0}}, 
                                             {{0, 1}, {0, 1}},
                                             {{1, 0}, {0, 1}}};

    constexpr static Vertex2D SQUARE[6] = {{{0, 0}, {0, 0}}, 
                                           {{1, 0}, {1, 0}},
                                           {{0, 1}, {0, 1}},
                                           {{1, 0}, {1, 0}},
                                           {{1, 1}, {1, 1}},
                                           {{0, 1}, {0, 1}}};
};