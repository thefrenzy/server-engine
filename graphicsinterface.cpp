#include "Panel.h"
#include "Logger.h"
#include "mapmanager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <sstream>
#include <atomic>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

extern std::atomic<bool> g_running;

void RunGraphicsPanel(MapState &ms) {
    if (!glfwInit()) return;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Server Engine Monitor", nullptr, nullptr);
    if (!window) { 
        glfwTerminate(); 
        return; 
    }

    GLFWimage icons[1];
    icons[0].pixels = stbi_load("icon.png", &icons[0].width, &icons[0].height, 0, 4); 
    
    if (icons[0].pixels) {
        glfwSetWindowIcon(window, 1, icons);
        stbi_image_free(icons[0].pixels);
    } else {
        Logger::Log("[ERROR] Could not find icon.png");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char commandInput[256] = {};
    const ImVec4 GREEN = ImVec4(0.1f, 0.9f, 0.2f, 1.0f);

    while (!glfwWindowShouldClose(window) && g_running) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("###Background", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoSavedSettings);
        ImGui::PopStyleVar(2);

        float footer = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 2.0f;

        if (ImGui::BeginChild("ScrollingLog", ImVec2(0, -footer), true, ImGuiWindowFlags_HorizontalScrollbar)) {
            const auto logs = Logger::GetLogs();
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(logs.size()));
            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                    ImGui::TextColored(GREEN, "%s", logs[i].c_str());
                }
            }
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 3.0f)
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::PushItemWidth(-FLT_MIN);
        if (ImGui::InputText("##Command", commandInput, IM_ARRAYSIZE(commandInput), ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string cmd = commandInput;
            if (!cmd.empty()) {
                Logger::Log("[ADMIN] " + cmd);
                ExecuteCommand(cmd, ms);
            }
            commandInput[0] = '\0';
            ImGui::SetKeyboardFocusHere(-1);
        }
        ImGui::PopItemWidth();

        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    g_running = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}