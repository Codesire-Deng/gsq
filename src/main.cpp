#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
// #include "imgui/imgui_impl_glfw.h"
// #include "imgui/imgui_impl_opengl3.h"

#include <boost/gil.hpp>

#include <iostream>
#include <optional>
#include <algorithm>
#include <filesystem>
#include <version/version.hpp>
#include <config/font.hpp>
#include <config/config.hpp>

#include <Eigen/Dense>
#include <convex.hpp>
#include <shader.hpp>
#include <bound.hpp>

static void glfw_error_callback(int error, const char *description);
static void
framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void processInput(GLFWwindow *window, const ImGuiIO &io);

// settings
constexpr unsigned int SCR_WIDTH = 1000;
constexpr unsigned int SCR_HEIGHT = 1000;

static GLFWwindow *initWindow();
static ImGuiIO &initImGui(GLFWwindow *window);

namespace Work {

int verticesNum, queryPolygonsNum;

GLuint pointsVBO, pointsVAO;
GLuint shaderProgram;

static void input();
static void workload();

} // namespace Work

int main() {
    GLFWwindow *window = initWindow();

    ImGuiIO &io = initImGui(window);

    printGLVersion();

    // Our state
    bool show_demo_window = false;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Work::input();

    glEnable(GL_PROGRAM_POINT_SIZE);

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
            ImGui::Begin("Hello, world!"); // Create a window called "Hello,
                                           // world!" and append into it.

            ImGui::Text(
                "This is some useful text."); // Display some text (you can use
                                              // a format strings too)
            ImGui::Checkbox(
                "Demo Window", &show_demo_window); // Edit bools storing our
                                                   // window open/close state
            /*
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat(
                "float", &f, 0.0f,
                1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            */
            ImGui::ColorEdit3(
                "clear color",
                (float *)&clear_color); // Edit 3 floats representing a color

            /*
            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets
                               // return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            */

            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("glfwGetTime %.3f s", glfwGetTime());
            ImGui::End();
        }

        // 3. Show another simple window.
        /*
        if (show_another_window) {
            ImGui::Begin(
                "Another Window",
                &show_another_window); // Pass a pointer to our bool variable
                                       // (the window will have a closing button
                                       // that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }
        */

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
            clear_color.x * clear_color.w, clear_color.y * clear_color.w,
            clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        Work::workload();

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

static void workload() {
    glUseProgram(shaderProgram);
    glBindVertexArray(pointsVAO);
    const int num = int(glfwGetTime()) % verticesNum + 1;
    glDrawArrays(GL_POINTS, 0, num);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, num);
}

static void input() {
    // construct shader and program
    // vShader.emplace(VertexShader("shader/point.vert"));
    // fShader.emplace(FragmentShader("shader/point.frag"));
    VertexShader vShader("shader/point.vert");
    FragmentShader fShader("shader/point.frag");
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);
#ifdef DEBUG
    {
        using namespace std;
        static int success;
        static char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
            cerr << "create_shader: compile failed:\n" << infoLog << endl;
            exit(1);
        }
    }
#endif

    constexpr int MAX_VERT = 32;
    using Point = Eigen::Matrix<real, 3, 1>;
    static Point points[MAX_VERT];
    Bound<real> bound[2];

    FILE *in = fopen("data/select_points.txt", "r");

    // input points to be queried
    fscanf(in, "%d", &verticesNum);
    for (int i = 0; i < verticesNum; ++i) {
        float x, y;
        fscanf(in, "%f%f", &x, &y);
        points[i] << x, y, 0.0f;
        bound[0].merge(x);
        bound[1].merge(y);
    }

    for (int i = 0; i < 2; ++i) {
        bound[i].lowerBound -= 5.0f;
        bound[i].upperBound += 5.0f;
    }

    const real ratio = (real)std::min<double>(
        1.0 / (bound[0].upperBound - bound[0].lowerBound) * (1.0f - (-1.0f)),
        1.0 / (bound[1].upperBound - bound[1].lowerBound) * (1.0f - (-1.0f)));

    // scale all coordinates to [-1.0, 1.0]
    const auto &lerp = [&](Point &point) {
        for (int i = 0; i < 2; ++i) {
            point[i] = (point[i] - bound[i].lowerBound) * ratio - 1.0f;
        }
    };

    std::for_each_n(points, verticesNum, lerp);

    // input querying polygons, which are made convexs
    // fscanf(in, "%d", &queryPolygonsNum);
    // std::vector<Polygon::Convex> convexs;
    // for (int i = 0; i < queryPolygonsNum; ++i) {
    //     convexs.push_back(Polygon::Convex::FromInput(in));
    //     auto [xBound, yBound] = convexs.back().bounds();
    //     bound[0].merge(xBound);
    //     bound[1].merge(yBound);
    // }

    // clang-format off
    static const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    // clang-format on

    glGenBuffers(1, &pointsVBO);
    glGenVertexArrays(1, &pointsVAO);
    glBindVertexArray(pointsVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(
        GL_ARRAY_BUFFER, verticesNum * sizeof(Point), points, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
    // GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    return window;
}

static ImGuiIO &initImGui(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
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
        for (const char *filename : Config::fontFamily) {
            if (std::filesystem::exists(filename)
                && io.Fonts->AddFontFromFileTTF(filename, 18.0f) != nullptr) {
                found = true;
                break;
            }
        }
        if (!found) io.Fonts->AddFontDefault();
    }

    return io;
}
