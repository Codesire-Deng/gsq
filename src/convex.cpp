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

std::pair<Bound<float>, Bound<float>> Convex::bounds() const {
    Bound<float> bound[2];
    for (const auto &v : vertices) {
        bound[0].merge(v.x());
        bound[1].merge(v.y());
    }
    return std::make_pair(bound[0], bound[1]);
}

} // namespace Polygon
