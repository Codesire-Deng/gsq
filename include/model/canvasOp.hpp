#pragma once

#include <glad/glad.h>
#include <config/type.hpp>
#include <config/data.hpp>
#include <program.hpp>
#include <convex.hpp>

namespace CanvasOp {

extern Polygon::Convex fullScreenConvex;

template<
    typename Size = Config::Type::Size2D<Config::SCR_WIDTH, Config::SCR_HEIGHT>>
inline Canvas<Size> &blendPointsWithPolygons(
    Canvas<Size> &result,
    const Canvas<Size> &points,
    const Canvas<Size> &polygons) {
    static Program context{
        VertexShader("shader/point.vert"),
        FragmentShader("shader/canvasBlendPointsWithPolygons.frag"),
    };
    context.use();
    result.bind(0, Access::writeOnly);
    points.bindConst(1);
    polygons.bindConst(2);
    fullScreenConvex.draw();
}

template<
    typename Size = Config::Type::Size2D<Config::SCR_WIDTH, Config::SCR_HEIGHT>>
inline Canvas<Size> &blendPolygons(
    Canvas<Size> &result,
    const Canvas<Size> &polygons0,
    const Canvas<Size> &polygons1) {
    static Program context(
        VertexShader("shader/point.vert"),
        FragmentShader("shader/canvasBlendPolygons.frag")
    );
    context.use();
    result.bind(0, Access::writeOnly);
    polygons0.bindConst(1);
    polygons1.bindConst(2);
    fullScreenConvex.draw();
    return result;
}

} // namespace CanvasOp