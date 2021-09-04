#pragma once

#include <program.hpp>

class Mesh {
  public:
    virtual ~Mesh(){};
    virtual void genVAO() = 0;
    // virtual void bindVAO() = 0;
    virtual void draw() const = 0;

    Program program;

    inline void useProgram() const { program.use(); }
};
