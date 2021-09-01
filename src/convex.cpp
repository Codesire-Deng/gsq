#include <convex.hpp>
#include <iostream>

namespace Polygon {
    Convex Convex::FromInput(FILE *const in) {
        Convex result;
        int verticesNum;
        fscanf(in, "%d", &verticesNum);
        result.vertices.resize(verticesNum);
        for (int i = 0, idx = 0, step = 2; i < verticesNum; ++i) {
            float x, y;
            fscanf(in, "%f%f", &x, &y);
            result.vertices[idx] << x, y, 0.0f;
            idx += step;
            if (idx == verticesNum) {
                --idx;
                step = -2;
            } else if (idx > verticesNum) {
                idx = verticesNum - 2;
                step = -2;
            }
        }

        return result;
    }

} // namespace Polygon
