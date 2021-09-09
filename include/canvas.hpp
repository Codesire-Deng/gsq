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
    typename Size = Config::Type::Size2D<Config::SCR_WIDTH, Config::SCR_HEIGHT>>
class Canvas final {
  public:
    static constexpr int COLUMN_SIZE = 3;
    static constexpr GLenum INTERNAL_FORMAT = GL_RGBA32I;
    static constexpr GLenum FORMAT = GL_RGBA_INTEGER;
    static constexpr GLenum TYPE = GL_INT;

    Canvas() {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &textureId);
        glTextureStorage3D(
            textureId,
            1,
            INTERNAL_FORMAT,
            Size::width,
            Size::height,
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

    inline Config::Type::SData readS(GLint xOffset, GLint yOffset) const {
        static int sColumn[4];
        Config::Type::SData result;
        const auto &copyColomn = [&,this](int column) {
            checkGlError(glGetTextureSubImage(
                textureId,
                0,
                xOffset,
                yOffset,
                column,
                1,
                1,
                1,
                GL_RGBA,
                TYPE,
                sizeof(sColumn),
                sColumn));
            for (int i = 0; i < 3; ++i) result.coeffRef(i, column) = sColumn[i];
        };
        copyColomn(0);
        copyColomn(1);
        copyColomn(2);
        return result;
    }

  private:
    GLuint textureId = 0;
};