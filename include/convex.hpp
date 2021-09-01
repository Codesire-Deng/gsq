#pragma once

#include <Eigen/Dense>
#include <vector>
#include <iostream>

namespace Polygon {

    class Convex {
      public:
        using Vertex = Eigen::Vector3f;

        static Convex FromInput(FILE *const in);

        Convex(const Convex &other) = default;
        Convex(Convex &&other) = default;

      protected:
        std::vector<Vertex> vertices;

      private:
        Convex(){};
    };

} // namespace Polygon