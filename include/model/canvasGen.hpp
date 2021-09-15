#pragma once

#include <uniform.hpp>

class UCanvasGen : public Uniform {
  public:
    virtual UCanvasGen &initLocation() override;

    UCanvasGen &sendCustomColor(const float color[]);
    UCanvasGen &setSRow0(const int data[]);
    UCanvasGen &setSRow1(const int data[]);
    UCanvasGen &setSRow2(const int data[]);
    UCanvasGen &setS(const int data[][3]);
    UCanvasGen &sendS();

  protected:
    // frag
    GLuint lCustomColor;
    GLuint lSColumn0;
    GLuint lSColumn1;
    GLuint lSColumn2;

    int sData[3][3];
};