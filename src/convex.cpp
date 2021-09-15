#include <glad/glad.h>
#include <convex.hpp>
#include <iostream>

namespace Polygon {
[[nodiscard]] Convex Convex::fromInput(FILE *const in) {
    Convex result;
    int verticesNum;
    fscanf(in, "%d", &verticesNum);
    result.vertices.resize(verticesNum);
    for (int i = 0, idx = 0, step = 2; i < verticesNum; ++i) {
        float x, y;
        fscanf(in, "%f%f", &x, &y);
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



[[nodiscard]] Bound2D Convex::bound() const {
    Bound2D result;
    for (const auto &v : vertices) {
        result.bounds[0].merge(v.x());
        result.bounds[1].merge(v.y());
    }
    return result;
}

void Convex::genVAO() {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.front().data(),
        GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(real), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Convex::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
    glBindVertexArray(0);
}

} // namespace Polygon
