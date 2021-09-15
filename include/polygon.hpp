#pragma once

#include <convex.hpp>
#include <vector>
#include <concepts>
#include <iostream>

namespace Polygon {

using namespace Config::Type;

class Polygon final : public Mesh {
  public:
    Polygon() {}

    Polygon(std::vector<Convex> convexs) : convexs(std::move(convexs)) {}

    Polygon(const Polygon &other) : convexs(other.convexs) {}

    Polygon(Polygon &&other) : convexs(std::move(other.convexs)) {}

    static Polygon fromInput(FILE *const in);

    Polygon &operator=(const Polygon &other) {
        if (this == &other) return *this;
        convexs = other.convexs;
        return *this;
    }

    Polygon &operator=(Polygon &&other) {
        if (this == &other) return *this;
        convexs = std::move(other.convexs);
        return *this;
    }

    virtual void genVAO() override {
        for (auto &c : convexs) { c.genVAO(); }
    }

    virtual void draw() const override {
        for (auto &c : convexs) { c.draw(); }
    }

    Bound2D bound() const;

    template<typename F>
    Polygon &adjustVertices(const F &f) requires std::invocable<F, Vertex &> {
        for (auto &c : convexs) { c.adjustVertices(f); }
        return *this;
    }

  private : std::vector<Convex> convexs;
};

} // namespace Polygon