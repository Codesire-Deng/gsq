#pragma once

#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <bound.hpp>
#include <config/config.hpp>
#include <mesh.hpp>

namespace Polygon {

class Convex : public Mesh {
  public:
    static Convex FromInput(FILE *const in);

    Convex(const Convex &other) = default;
    Convex(Convex &&other) = default;

    std::pair<Bound<real>, Bound<real>> bounds() const;

    virtual void genVAO() override;
    virtual void draw() const override;

  protected:
    std::vector<Vertex> vertices;
    unsigned int VAO = 0;

  public:
    inline const decltype(vertices) &pubVertices() const { return vertices; }
    inline decltype(vertices) &pubVertices() { return vertices; }

  private:
    Convex(){};
};

} // namespace Polygon