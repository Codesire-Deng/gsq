#pragma once

#include <glad/glad.h>
#include <unordered_map>
#include <string>

class Shader {
  public:
    inline GLuint id() const { return shaderId; }
    inline operator GLuint() const { return id(); };
    virtual ~Shader();
    void clear();

    const std::string filename;

  protected:
    GLuint shaderId;
    Shader(std::string filename, GLenum type);
    Shader() = delete;
    Shader(const Shader &other) = delete;
    Shader(Shader &&other) = default;

  private:
    static std::unordered_map<std::string, GLuint> shaderCache;
};

class VertexShader : public Shader {
  public:
    VertexShader(std::string filename);
    VertexShader() = delete;
    VertexShader(const VertexShader &other) = delete;
    VertexShader(VertexShader &&other) = default;
};

class FragmentShader : public Shader {
  public:
    FragmentShader(std::string filename);
    FragmentShader() = delete;
    FragmentShader(const FragmentShader &other) = delete;
    FragmentShader(FragmentShader &&other) = default;
};
