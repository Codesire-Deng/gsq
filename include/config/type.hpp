#pragma once

#include <glad/glad.h>
#include <bound.hpp>
#include <Eigen/Dense>

namespace Config {

namespace Type {
    using real = float;
    using Point = Eigen::Matrix<real, 2, 1>;
    using Vertex = Point;
    using Color3f = Eigen::Vector3f;
    using Color4f = Eigen::Vector4f;
    using Bound2D = Bounds<real, 2>;

    using SData = Eigen::Matrix3i;

    template<int width, int height>
    struct Size2D final {
        static constexpr int width = width;
        static constexpr int height = height;
    };

    template<int width, int height, int depth>
    struct Size3D final {
        static constexpr int width = width;
        static constexpr int height = height;
        static constexpr int depth = depth;
    };

    struct Offset2D final {
        int x, y;
        bool operator!=(Offset2D other) {
            return x != other.x || y != other.y;
        }
    };

} // namespace Type

} // namespace Config
