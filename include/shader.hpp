#pragma once

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <file_buffer.hpp>

namespace impl_Shader {

enum class ShaderType : GLenum {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
};

// from data.cpp
extern std::unordered_map<std::string, GLuint> shaderCache;

inline GLuint createShader(std::string_view filename, ShaderType type) {
    GLuint shaderId = glCreateShader((GLenum)type);
    std::string source(FileBuffer::binaryFromFile(filename));
    const auto data = source.data();
    glShaderSource(shaderId, 1, &data, nullptr);
    glCompileShader(shaderId);
#ifdef DEBUG
    using namespace std;
    static int success;
    static char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        cerr << "createShader: compile failed:" << filename << "\n"
             << infoLog << endl;
        exit(1);
    }
#endif
    return shaderId;
}

template<ShaderType shaderType>
class Shader final {
  public:
    inline GLuint id() const { return shaderId; }

    inline operator GLuint() const { return id(); };

    inline ~Shader() {
        if (shaderId != 0) {
            clear();
            shaderId = 0;
        }
    }

    inline void clear() {
        if (filename.empty()) return;
        glDeleteShader(shaderId);
        shaderCache.erase(filename);
    }

  public:
    Shader() = delete;

    // 自带缓存
    inline Shader(std::string filename) : shaderId(0), filename(std::move(filename)) {
        const std::string &name = this->filename;
        auto it = shaderCache.find(name);
        if (it != shaderCache.end()) {
            shaderId = it->second;
        } else {
            shaderId = createShader(name, shaderType);
            shaderCache.insert({name, shaderId});
        }
    }

    Shader(const Shader &other) = delete;

    inline Shader(Shader &&other)
        : shaderId(other.shaderId), filename(std::move(other.filename)) {
        other.shaderId = 0;
    };

    Shader &operator=(const Shader &other) = delete;

    inline Shader &operator=(Shader &&other) {
        shaderId = other.shaderId;
        filename = std::move(other.filename);
        other.shaderId = 0;
    }

  private:
    GLuint shaderId;
    std::string filename;
};

} // namespace impl_Shader

using VertexShader = impl_Shader::Shader<impl_Shader::ShaderType::vertex>;
using FragmentShader = impl_Shader::Shader<impl_Shader::ShaderType::fragment>;
