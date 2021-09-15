#include <glad/glad.h>
#include <polygon.hpp>

namespace Polygon {
[[nodiscard]] Polygon Polygon::fromInput(FILE *const in) {
    Polygon result;
    int convexsNum;
    fscanf(in, "%d", &convexsNum);
    result.convexs.reserve(convexsNum);
    for (int i = 0; i < convexsNum; ++i) {
        result.convexs.push_back(Convex::fromInput(in));
    }
    return result;
}


[[nodiscard]] Bound2D Polygon::bound() const {
    Bound2D result;
    for (const auto &c : convexs) {
        result.merge(c.bound());
    }
    return result;
}

} // namespace Polygon
