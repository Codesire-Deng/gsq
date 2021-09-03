#pragma once



class Mesh {
  public:
    virtual ~Mesh(){};
    virtual void genVAO() = 0;
    // virtual void bindVAO() = 0;
    virtual void draw() const = 0;

    unsigned int program = 0;
};
