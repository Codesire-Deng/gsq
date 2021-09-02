#include <shader.hpp>
#include <iostream>
#include <file_buffer.hpp>

std::unordered_map<std::string, GLuint> Shader::shaderCache;

static GLuint create_shader(std::string_view filename, GLenum type) {
    GLuint shaderId = glCreateShader(type);
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
        cerr << "create_shader: compile failed:" << filename << "\n"
             << infoLog << endl;
        exit(1);
    }
#endif
    return shaderId;
}

Shader::~Shader() {
    clear();
}

// 自带缓存
Shader::Shader(std::string filename, GLenum type)
    : shaderId(0), filename(std::move(filename)) {
    using namespace std;
    const std::string &name = this->filename;
    auto it = shaderCache.find(name);
    if (it != shaderCache.end()) {
        shaderId = it->second;
    } else {
        shaderId = create_shader(name, type);
        shaderCache.insert({name, shaderId});
    }
}

void Shader::clear() {
    if (filename.empty()) return;
    glDeleteShader(shaderId);
    shaderCache.erase(filename);
}

// 自带缓存
VertexShader::VertexShader(std::string filename)
    : Shader(filename, GL_VERTEX_SHADER) {
}

// 自带缓存
FragmentShader::FragmentShader(std::string filename)
    : Shader(filename, GL_FRAGMENT_SHADER) {
}
