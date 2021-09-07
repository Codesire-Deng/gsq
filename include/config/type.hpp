#pragma once

#include <Eigen/Dense>

namespace Config {

namespace Type {
    using real = float;
    using Point = Eigen::Matrix<real, 2, 1>;
    using Vertex = Point;
    using Color3f = Eigen::Vector3f;
    using Color4f = Eigen::Vector4f;
} // namespace Type

} // namespace Config
