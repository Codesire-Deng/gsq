#pragma once

#include <glad/glad.h>
#include <config/type.hpp>
#include <config/gl.hpp>
#ifdef DEBUG
#include <iostream>
#include <cassert>
#endif

enum class Access : GLenum {
    readOnly = GL_READ_ONLY,
    writeOnly = GL_WRITE_ONLY,
    readWrite = GL_READ_WRITE
};

template<
    typename Size2D =
        Config::Type::Size2D<Config::SCR_WIDTH, Config::SCR_HEIGHT>>
class Canvas final {
  public:
    static constexpr int COLUMN_SIZE = 3;
    static constexpr int COLUMN_LENGTH = 4; // RGBA
    static constexpr GLenum INTERNAL_FORMAT = GL_RGBA32I;
    static constexpr GLenum FORMAT = GL_RGBA_INTEGER;
    static constexpr GLenum TYPE = GL_INT;

    Canvas() {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &textureId);
        glTextureStorage3D(
            textureId,
            1,
            INTERNAL_FORMAT,
            Size2D::width,
            Size2D::height,
            COLUMN_SIZE);
        clear();
    }

    ~Canvas() {
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }
    }

    Canvas(const Canvas &other) = delete;

    Canvas(Canvas &&other) {
        textureId = other.textureId;
        other.textureId = 0;
    }

    Canvas &operator=(const Canvas &other) = delete;

    Canvas &operator=(Canvas &&other) {
        if (this == &other) return *this;
        textureId = other.textureId;
        other.textureId = 0;
        return *this;
    }

    inline Canvas &clear() {
        glClearTexImage(textureId, 0, FORMAT, TYPE, Config::Data::zero);
        return *this;
    }

    inline Canvas &bind(GLuint unit, Access access) {
        glBindImageTexture(
            unit, textureId, 0, GL_TRUE, 0, (GLenum)access, INTERNAL_FORMAT);
        return *this;
    }

    inline const Canvas &bindConst(GLuint unit) const {
        glBindImageTexture(
            unit,
            textureId,
            0,
            GL_TRUE,
            0,
            (GLenum)Access::readOnly,
            INTERNAL_FORMAT);
        return *this;
    }

    inline Config::Type::SData readS(GLint xOffset, GLint yOffset) const {
        static int sColumn[COLUMN_SIZE][COLUMN_LENGTH];
        Config::Type::SData result;
        showGlError();
        glGetTextureSubImage(
            textureId,
            0,
            xOffset,
            yOffset,
            0,           // zOffset
            1,           // width
            1,           // height
            COLUMN_SIZE, // depth
            FORMAT,
            TYPE,
            sizeof(sColumn),
            sColumn);
        showGlError();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < COLUMN_SIZE; ++j)
                result.coeffRef(i, j) = sColumn[j][i];
        return result;
    }

  public:
  private:
    GLuint textureId = 0;
};