#pragma once

#include <glad/glad.h>
#include <shader.hpp>
#include <concepts>
#ifdef DEBUG
#include <iostream>
#include <cassert>
#endif

class Program final {
  public:
    operator GLuint() const { return programId; }

    Program(){};

    template<typename... S>
    requires std::convertible_to<S..., GLuint>
    explicit Program(const S &...shader) : programId(glCreateProgram()) {
        attach(shader...);
        link();
        assertAvailable();
    }

    template<typename S, typename... Ss>
    Program &attach(const S &shader, const Ss &...shaders) requires
        std::convertible_to<S, GLuint> {
        glAttachShader(programId, shader);
        if constexpr (sizeof...(shaders) > 0) { attach(shaders...); }
        return *this;
    }

    inline Program &init() {
        assert(programId == 0 && "Program: duplicate init");
        programId = glCreateProgram();
        return *this;
    }

    inline Program &link() {
        glLinkProgram(programId);
        return *this;
    }

    inline void use() const { glUseProgram(programId); }

    void assertAvailable() const {
#ifdef DEBUG
        using namespace std;
        assert(programId != 0);
        static int success;
        static char infoLog[512];
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(programId, 512, nullptr, infoLog);
            cerr << "createShader: compile failed:\n" << infoLog << endl;
            exit(1);
        }
#endif
    };

  private:
    GLuint programId = 0;
};