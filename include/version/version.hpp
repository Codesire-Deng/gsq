#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void printGLVersion() {
    const GLubyte *name =
        glGetString(GL_VENDOR);
    const GLubyte *identfiier =
        glGetString(GL_RENDERER);
    const GLubyte *OpenGLVersion =
        glGetString(GL_VERSION);
    const GLubyte *GLSLVersion =
        glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("OpenGL vendor: %s\n", name);
    printf("Renderer id: %s\n", identfiier);
    printf("OpenGL version: %s\n", OpenGLVersion);
    printf("GLSL version: %s\n", GLSLVersion);
}