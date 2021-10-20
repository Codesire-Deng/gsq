#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
// #include "imgui/imgui_impl_glfw.h"
// #include "imgui/imgui_impl_opengl3.h"
// #include <boost/gil.hpp>
#include <Eigen/Dense>

#include <iostream>
#include <optional>
#include <algorithm>
#include <filesystem>
#include <ranges>

#include <config/data.hpp>
#include <config/type.hpp>
#include <config/gl.hpp>
#include <version/version.hpp>

#include <shader.hpp>
#include <program.hpp>
#include <uniform.hpp>

#include <bound.hpp>
#include <polygon.hpp>
#include <convex.hpp>
#include <canvas.hpp>

#include <model/canvasGen.hpp>
#include <model/canvasOp.hpp>

static void glfw_error_callback(int error, const char *description);
static void
framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void processInput(GLFWwindow *window, const ImGuiIO &io);

static GLFWwindow *initWindow();
static ImGuiIO &initImGui(GLFWwindow *window);

namespace Work {

using namespace Config::Type;

int verticesNum, queryPolygonsNum;

Color3f pointColor(1.0f, 0.5f, 0.2f);
Color3f polygonColor(1.0f, 1.0f, 0.5f);
Color3f polygon1Color(1.0f, 0.5f, 0.5f);
Color3f polygon2Color(0.5f, 0.5f, 1.0f);

GLuint pointsVBO, pointsVAO;
Program pointProgram, canvasGenProgram;
UCanvasGen uCanvasGen;
Config::Type::SData mouseSData;
Config::Type::Offset2D mouse, mouseLast;
float pointSize = 10.0f;
int customColorLocation = -1;
Bound2D bound2D;

Polygon::Polygon p0, p1, p2;



static void inputPoints();
static void inputPolygons();
static void input();
static void workloadPoints();
static void workloadPolygons();
static void workload();

} // namespace Work

int main() {
    GLFWwindow *window = initWindow();
    ImGuiIO &io = initImGui(window);
    printGLVersion();
    logGlIsEnabled(GL_SCISSOR_TEST);

    // Our state
    bool show_demo_window = false;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    checkGlError(Work::input());

    // glEnable(GL_PROGRAM_POINT_SIZE);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
        // tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data
        // to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
        // data to your main application. Generally you may always pass all
        // inputs to dear imgui, and hide them from your application based on
        // those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in
        // ImGui::ShowDemoWindow()! You can browse its code to learn more about
        // Dear ImGui!).
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End
        // pair to created a named window.
        {
            // static float f = 0.0f;
            // static int counter = 0;
            ImGui::Begin("Debug Info"); // Create a window called "Hello,
                                        // world!" and append into it.

            ImGui::Checkbox(
                "Demo Window", &show_demo_window); // Edit bools storing our
                                                   // window open/close state

            // ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat(
                "Point size",
                &Work::pointSize,
                1.0f,
                10.0f); // Edit 1 float using a slider from 0.0f to 1.0f

            ImGui::ColorEdit3(
                "Point color",
                Work::pointColor.data()); // Edit 3 floats representing a color

            ImGui::ColorEdit3(
                "Polygon color",
                Work::polygonColor
                    .data()); // Edit 3 floats representing a color
            ImGui::ColorEdit3(
                "Polygon1 color",
                Work::polygon1Color
                    .data()); // Edit 3 floats representing a color
            ImGui::ColorEdit3(
                "Polygon2 color",
                Work::polygon2Color
                    .data()); // Edit 3 floats representing a color

            ImGui::ColorEdit3(
                "Clear color",
                (float *)&clear_color); // Edit 3 floats representing a color

            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
            ImGui::Text("glfwGetTime %.3f s", glfwGetTime());

            if (int mx = (int)io.MousePos.x, my = (int)io.MousePos.y;
                ImGui::IsMousePosValid() && !io.WantCaptureMouse && 0 <= mx
                && mx < Config::SCR_WIDTH && 0 <= my
                && my < Config::SCR_HEIGHT) {
                using namespace Work;
                mouse.x = mx;
                mouse.y = Config::SCR_HEIGHT - 1 - my;
                ImGui::Text("Mouse pos: (%d, %d)", mouse.x, mouse.y);
                ImGui::Text(
                    "[%4d%4d%4d]\n[%4d%4d%4d]\n[%4d%4d%4d]",
                    mouseSData.coeff(0, 0),
                    mouseSData.coeff(0, 1),
                    mouseSData.coeff(0, 2),
                    mouseSData.coeff(1, 0),
                    mouseSData.coeff(1, 1),
                    mouseSData.coeff(1, 2),
                    mouseSData.coeff(2, 0),
                    mouseSData.coeff(2, 1),
                    mouseSData.coeff(2, 2));
            } else {
                ImGui::Text("Mouse pos: <INVALID>");
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
            clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // IMPORTENT: Entrance to workload
        checkGlError(Work::workload());

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

namespace Work {

static void workloadPoints() {
    static Canvas canvas;

    showGlError();

    // clang-format off
    static const int sData[3][3] = {
        {1, 1, 0},
        {2, 1, 0},
        {3, 1, 0},
    };
    // clang-format on

    uCanvasGen.useProgram();
    uCanvasGen.sendCustomColor(polygonColor.data()).setS(sData).sendS();
    showGlError();
    checkGlError(canvas.bind(0, Access::writeOnly));
    // draw convexs
    p0.draw();

    showGlError();

    pointProgram.use();
    glUniform3fv(customColorLocation, 1, pointColor.data());
    // draw points
    {
        glBindVertexArray(pointsVAO);
        glPointSize(pointSize);
        glDrawArrays(GL_POINTS, 0, verticesNum);
    }

    showGlError();

    if (mouse != mouseLast) {
        mouseSData = canvas.readS(mouse.x, mouse.y);
        mouseLast = mouse;
    }
}

static void workloadPolygons() {
    static Canvas result, blend1, blend2;
    showGlError();
    // clang-format off
    static const int sData1[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {1, 1, 0},
    };
    static const int sData2[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {2, 1, 0},
    };
    // clang-format on

    // canvas for blend1
    uCanvasGen.useProgram();
    uCanvasGen.sendCustomColor(polygon1Color.data()).setS(sData1).sendS();
    showGlError();
    checkGlError(blend1.bind(0, Access::writeOnly));
    p1.draw();
    showGlError();

    // canvas for blend2
    uCanvasGen.useProgram();
    uCanvasGen.sendCustomColor(polygon2Color.data()).setS(sData2).sendS();
    showGlError();
    checkGlError(blend2.bind(0, Access::writeOnly));
    p2.draw();
    showGlError();

    CanvasOp::blendPolygons(result, blend1, blend2);
    if (mouse != mouseLast) {
        mouseSData = result.readS(mouse.x, mouse.y);
        mouseLast = mouse;
    }
}

static void workload() {
    // workloadPoints();
    workloadPolygons();
}

static void inputPoints() {
    using namespace std::views;
    constexpr int MAX_VERT = 32;
    static Point points[MAX_VERT];

    FILE *in = fopen("data/select_points.txt", "r");

    // input points to be queried
    fscanf(in, "%d", &verticesNum);
    for (float x, y; auto &point : points | take(verticesNum)) {
        fscanf(in, "%f%f", &x, &y);
        point << x, y;
        bound2D.bounds[0].merge(x);
        bound2D.bounds[1].merge(y);
    }

    // input querying polygons, which are made convexs
    p0 = Polygon::Polygon::fromInput(in);
    bound2D.merge(p0.bound());

    bound2D.slack(Config::BOUND_SLACK);
    const real ratio = (real)std::min<double>(
        1.0 / (bound2D.bounds[0].length()) * (1.0 - (-1.0)),
        1.0 / (bound2D.bounds[1].length()) * (1.0 - (-1.0)));

    // scale all coordinates to [-1.0, 1.0]
    const auto &lerp = [&](Point &point) {
        for (int i = 0; i < 2; ++i) {
            point[i] = (point[i] - bound2D.bounds[i].lowerBound) * ratio - 1.0f;
        }
    };

    std::for_each_n(points, verticesNum, lerp);
    p0.adjustVertices(lerp).genVAO();

    glGenBuffers(1, &pointsVBO);
    glGenVertexArrays(1, &pointsVAO);
    glBindVertexArray(pointsVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(
        GL_ARRAY_BUFFER, verticesNum * sizeof(Point), points, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
    // GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(real), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

static void inputPolygons() {
    FILE *in = fopen("data/select_polygons.txt", "r");
    p1 = Polygon::Polygon::fromInput(in);
    p2 = Polygon::Polygon::fromInput(in);
    bound2D.merge(p1.bound());
    bound2D.merge(p2.bound());
    bound2D.slack(Config::BOUND_SLACK);

    const real ratio = (real)std::min<double>(
        1.0 / (bound2D.bounds[0].length()) * (1.0 - (-1.0)),
        1.0 / (bound2D.bounds[1].length()) * (1.0 - (-1.0)));
    // scale all coordinates to [-1.0, 1.0]
    const auto &lerp = [&](Point &point) {
        for (int i = 0; i < 2; ++i) {
            point[i] = (point[i] - bound2D.bounds[i].lowerBound) * ratio - 1.0f;
        }
    };
    p1.adjustVertices(lerp).genVAO();
    p2.adjustVertices(lerp).genVAO();
}

static void input() {
    CanvasOp::fullScreenConvex.genVAO();

    // construct shader and program
    VertexShader pointVShader("shader/point.vert");
    FragmentShader pointFShader("shader/point.frag");
    pointProgram.init()
        .attachAndLink(pointVShader, pointFShader)
        .assertAvailable();

    VertexShader canvasGenVShader("shader/canvasGen.vert");
    FragmentShader canvasGenFShader("shader/canvasGen.frag");
    canvasGenProgram.init()
        .attachAndLink(canvasGenVShader, canvasGenFShader)
        .assertAvailable();

    // prepare to set color uniform
    customColorLocation = glGetUniformLocation(pointProgram, "customColor");
    uCanvasGen.setProgram(canvasGenProgram);

    // inputPoints();
    inputPolygons();
}

} // namespace Work

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// -----------------------------------------------------------------------------
static void processInput(GLFWwindow *window, const ImGuiIO &io) {
    if (!io.WantCaptureKeyboard) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// -----------------------------------------------------------------------------
static void
framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static GLFWwindow *initWindow() {
    // glfw: initialize and configure
    // ------------------------------
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return nullptr;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if constexpr (Config::ENABLE_MSAA) { glfwWindowHint(GLFW_SAMPLES, 4); }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(
        Config::SCR_WIDTH, Config::SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return -1;
        return nullptr;
    }

    if constexpr (Config::ENABLE_MSAA) { logGlEnable(GL_MULTISAMPLE); }

    glViewport(0, 0, Config::SCR_WIDTH, Config::SCR_HEIGHT);

    return window;
}

static ImGuiIO &initImGui(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // handle those errors in your application (e.g. use an assertion, or
    // display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and
    // stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame
    // below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !

    {
        bool found = false;
        auto exists = [&io](const char *filename) {
            return std::filesystem::exists(filename)
                   && io.Fonts->AddFontFromFileTTF(filename, 18.0f) != nullptr;
        };
        using namespace std::views;
        for (auto filename :
             Config::Data::fontFamily | filter(exists) | take(1)) {
            found = true;
        }
        if (!found) io.Fonts->AddFontDefault();
    }

    return io;
}
