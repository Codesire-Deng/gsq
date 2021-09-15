#pragma once

#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <bound.hpp>
#include <config/type.hpp>
#include <mesh.hpp>

namespace Polygon {

using namespace Config::Type;

class Convex : public Mesh {
  public:
    static Convex fromInput(FILE *const in);

    template<size_t N>
    static Convex fromArray(const float (&vertices)[N]);

    Convex(const Convex &other) : vertices(other.vertices), VAO(other.VAO) {}

    Convex(Convex &&other)
        : vertices(std::move(other.vertices)), VAO(other.VAO) {
        other.VAO = 0;
    }

    Convex &operator=(const Convex &other) {
        if (this == &other) return *this;
        vertices = other.vertices;
        VAO = other.VAO;
        return *this;
    }

    Convex &operator=(Convex &&other) {
        if (this == &other) return *this;
        vertices = std::move(other.vertices);
        VAO = other.VAO;
        other.VAO = 0;
        return *this;
    }

    Bound2D bound() const;

    virtual void genVAO() override;
    virtual void draw() const override;

    template<typename F>
    Convex &adjustVertices(const F &f) requires std::invocable<F, Vertex &> {
        for (auto &v : vertices) { f(v); }
        return *this;
    }

  protected:
    std::vector<Vertex> vertices;
    unsigned int VAO = 0;

  public:
    inline const decltype(vertices) &pubVertices() const { return vertices; }
    inline decltype(vertices) &pubVertices() { return vertices; }

  private:
    Convex(){};
};

template<size_t N>
[[nodiscard]] Convex Convex::fromArray(const float (&vertices)[N]) {
    Convex result;
    static_assert((N & 1) == 0);
    constexpr int verticesNum = N / 2;
    result.vertices.resize(verticesNum);
    for (int i = 0, idx = 0, step = 2; i < verticesNum; ++i) {
        const float x = vertices[i << 1];
        const float y = vertices[(i << 1) | 1];
        result.vertices[idx] << x, y;
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