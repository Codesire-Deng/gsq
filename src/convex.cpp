#include <glad/glad.h>
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

[[nodiscard]] std::pair<Bound<real>, Bound<real>> Convex::bounds() const {
    Bound<real> bound[2];
    for (const auto &v : vertices) {
        bound[0].merge(v.x());
        bound[1].merge(v.y());
    }
    return std::make_pair(bound[0], bound[1]);
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
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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
