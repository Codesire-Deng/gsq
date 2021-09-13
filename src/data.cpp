#include <config/data.hpp>
#include <unordered_map>

namespace Config {

namespace Data {
    // 16x32 bits
    const unsigned int zero[16] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    const char *const fontFamily[5] = {
        "c:\\Windows\\Fonts\\FiraCode-Retina.ttf",
        "C:\\Users\\ps_v5\\AppData\\Local\\Microsoft"
        "\\Windows\\Fonts\\FiraCode-Retina.ttf",
        "c:\\Windows\\Fonts\\consola.ttf",
        "c:\\Windows\\Fonts\\cour.ttf",
        "c:\\Windows\\Fonts\\arial.ttf"};
} // namespace Data

} // namespace Config

namespace impl_Shader {
using GLuint = unsigned int;
std::unordered_map<std::string, GLuint> shaderCache;
}