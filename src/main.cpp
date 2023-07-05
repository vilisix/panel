// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "ActionSet.h"
#include "Hotline.h"
#include "ArgProvider.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static std::vector<std::string> infoMessages;

void testFunctionZeroPar() {
    infoMessages.push_back("executed 0 param");
}

void testFunctionOnePar(const std::string &param1) {
    infoMessages.push_back("executed 1 param: " + param1);
}

void testFunctionTwoPar(const std::string &param1, int param2) {
    infoMessages.push_back("executed 2 param: " + param1 + " " + std::to_string(param2));
}

void testFunctionThreePar(const std::string &param1, int param2, bool param3) {
    infoMessages.push_back("executed 3 param: " + param1 + " " + std::to_string(param2) + " " + std::to_string(param3));
}


// Main code
int main(int, char **) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    int width = 1920;
    int height = 900;
    float scaleFactor = (width / 1280.f) * (height / 720.f);
    GLFWwindow *window = glfwCreateWindow(width, height, "hotline example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //  test action set for understanding how it works
    auto actionSet = std::make_shared<Hotline::ActionSet>();

    actionSet->AddAction("testZeroPar", testFunctionZeroPar);
    actionSet->AddAction("testOnePar", testFunctionOnePar,
                         ArgProvider<std::string>("Name"));
    actionSet->AddAction("testTwoPar", testFunctionTwoPar,
                         ArgProvider<std::string>("Name"),
                         ArgProvider<unsigned>("Level"));
    actionSet->AddAction("testThreePar", testFunctionThreePar,
                         ArgProvider<std::string>("Name"),
                         ArgProvider<int>("Level"),
                         ArgProvider<bool>("IsActive"));

    //  instantiation of hotline
    auto hotlineConfig = std::make_unique<Hotline::Config>();
    hotlineConfig->scaleFactor = scaleFactor;
    hotlineConfig->showRecentActions = true;
    // you can modify config as you like here
    auto hotline = std::make_unique<Hotline::Hotline>(actionSet, std::move(hotlineConfig));


    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Hotline main input and textInput update cycle
        hotline->Update();

        //  test info window
        ImGui::SetNextWindowPos({0.f, 0.f});
        ImGui::SetNextWindowBgAlpha(0.5f);
        ImGui::Begin("InfoWindow", 0, ImGuiWindowFlags_NoTitleBar
                                      | ImGuiWindowFlags_NoMove
                                      | ImGuiWindowFlags_AlwaysAutoResize
                                      | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetWindowFontScale(scaleFactor);
        ImGui::Text("Press F1 to open hotline");
        ImGui::Separator();
        for (const auto &message: infoMessages) {
            ImGui::Text(message.c_str());
        }
        ImGui::End();


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
