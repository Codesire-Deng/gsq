#pragma once

#include <glad/glad.h>
namespace Config {

constexpr unsigned int SCR_WIDTH = 1024;
constexpr unsigned int SCR_HEIGHT = 1024;

constexpr bool ENABLE_MSAA = true;

} // namespace Config

#define logGlIsEnabled(cap) printf("%s=%d\n", #cap, glIsEnabled(cap))

#define logGlEnable(cap)                              \
    do {                                              \
        printf("%s=%d->1\n", #cap, glIsEnabled(cap)); \
        glEnable(cap);                                \
    } while (0)

#define logGlDisable(cap)                             \
    do {                                              \
        printf("%s=%d->0\n", #cap, glIsEnabled(cap)); \
        glDisable(cap);                               \
    } while (0)

namespace Config {

bool impl_checkGlError() {
    const GLenum err = glGetError();
    if (err == 0) return false;
#ifdef DEBUG
    using namespace std;
    cerr << "[ERROR] glError=" << err << endl;
    switch (err) {
        case GL_INVALID_ENUM:
            cerr << "GL_INVALID_ENUM: "
                    "An unacceptable value "
                    "is specified for an enumerated argument. "
                    "The offending function is ignored, "
                    "having no side effect other than to "
                    "set the error flag.\n";
            break;
        case GL_INVALID_VALUE:
            cerr << "GL_INVALID_VALUE: "
                    "A numeric argument is out of range. "
                    "The offending function is ignored, "
                    "having no side effect other than to "
                    "set the error flag.\n";
            break;
        case GL_INVALID_OPERATION:
            cerr << "GL_INVALID_OPERATION: "
                    "The specified operation is not allowed "
                    "in the current state. "
                    "The offending function is ignored, "
                    "having no side effect other than to "
                    "set the error flag.\n";
            break;
        // case GL_STACK_OVERFLOW:
        //     cerr << "GL_STACK_OVERFLOW: "
        //             "This function would cause a stack overflow. "
        //             "The offending function is ignored, "
        //             "having no side effect other than to "
        //             "set the error flag.\n";
        //     break;
        // case GL_STACK_UNDERFLOW:
        //     cerr << "GL_STACK_UNDERFLOW: "
        //          << "This function would cause a stack underflow. "
        //             "The offending function is ignored, "
        //             "having no side effect other than to "
        //             "set the error flag.\n";
        //     break;
        case GL_OUT_OF_MEMORY:
            cerr << "GL_OUT_OF_MEMORY: "
                    "There is not enough memory left to "
                    "execute the function. "
                    "The state of OpenGL is undefined, "
                    "except for the state of the error flags, "
                    "after this error is recorded.\n";
            break;
        default:
            cerr << "Unknown glError number\n";
            break;
    }
#endif
    return true;
}

} // namespace Config

#ifdef DEBUG
#define checkGlError(callee)                                      \
    do {                                                          \
        while (glGetError() != GL_NO_ERROR) {                     \
            fprintf(                                              \
                stderr,                                           \
                "[WARNING] unhandled error before %s(%d)\n",      \
                __FILE__,                                         \
                __LINE__);                                        \
        }                                                         \
        callee;                                                   \
        const bool hasErr = Config::impl_checkGlError();          \
        if (!hasErr) break;                                       \
        fprintf(stderr, "Error at %s(%d)\n", __FILE__, __LINE__); \
        fprintf(stderr, "Function:   %s\n\n", __FUNCTION__);      \
        exit(1);                                                  \
    } while (0)
#else
#define checkGlError(callee) callee
#endif

#ifdef DEBUG
#define showGlError()                                             \
    do {                                                          \
        const bool hasErr = Config::impl_checkGlError();          \
        if (!hasErr) break;                                       \
        fprintf(stderr, "Error at %s(%d)\n", __FILE__, __LINE__); \
        fprintf(stderr, "Function:   %s\n\n", __FUNCTION__);      \
        exit(1);                                                  \
    } while (0)
#else
#define showGlError() (void)
#endif