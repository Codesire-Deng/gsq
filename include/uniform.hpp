#pragma once

#include <glad/glad.h>
#include <program.hpp>

class Uniform {
  public:
    Uniform() {}

    inline Uniform &setProgram(Program program) {
        this->program = program;
        initLocation();
        return *this;
    }

    virtual Uniform &initLocation() { return *this; }

    inline Uniform &useProgram() { program.use(); return *this; }

  protected:
    Program program;
};