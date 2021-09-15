#include <model/canvasGen.hpp>

UCanvasGen &UCanvasGen::initLocation() {
    lCustomColor = glGetUniformLocation(program, "customColor");
    lSColumn0 = glGetUniformLocation(program, "sColumn0");
    lSColumn1 = glGetUniformLocation(program, "sColumn1");
    lSColumn2 = glGetUniformLocation(program, "sColumn2");
    return *this;
}

UCanvasGen &UCanvasGen::sendCustomColor(const float color[]) {
    glUniform3fv(lCustomColor, 1, color);
    return *this;
}

UCanvasGen &UCanvasGen::setSRow0(const int data[]) {
    for (int i = 0; i < 3; ++i) { sData[i][0] = data[i]; }
    return *this;
}

UCanvasGen &UCanvasGen::setSRow1(const int data[]) {
    for (int i = 0; i < 3; ++i) { sData[i][1] = data[i]; }
    return *this;
}

UCanvasGen &UCanvasGen::setSRow2(const int data[]) {
    for (int i = 0; i < 3; ++i) { sData[i][2] = data[i]; }
    return *this;
}

UCanvasGen &UCanvasGen::setS(const int data[][3]) {
    setSRow0(data[0]);
    setSRow1(data[1]);
    setSRow2(data[2]);
    return *this;
}

UCanvasGen &UCanvasGen::sendS() {
    glUniform3iv(lSColumn0, 1, sData[0]);
    glUniform3iv(lSColumn1, 1, sData[1]);
    glUniform3iv(lSColumn2, 1, sData[2]);
    return *this;
}
