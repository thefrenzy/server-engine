#include "Panel.h"
#include "Logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

void RunGraphicsPanel(MapState &ms) {
    if (!glfwInit()) return;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Server Engine Monitor", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    char commandInput[256] = {};

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

        ImGui::Begin("###Background", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoSavedSettings);

        ImGui::PopStyleVar(3);

        float footer = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 2.0f;

        ImGui::BeginChild("ScrollingLog", ImVec2(0, -footer), true, ImGuiWindowFlags_HorizontalScrollbar);

        const auto& logs = Logger::GetLogs();

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(logs.size()));
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                ImGui::TextUnformatted(logs[i].c_str());
            }
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 2.0f) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        ImGui::Separator();

        bool reclaim = false;
        ImGui::PushItemWidth(-FLT_MIN);
        if (ImGui::InputText("Command", commandInput, IM_ARRAYSIZE(commandInput),
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string cmd = commandInput;
            if (!cmd.empty()) {
                Logger::Log("[SERVER] " + cmd);
                ExecuteCommand(cmd, ms);
            }
            commandInput[0] = '\0';
            reclaim = true;
        }
        ImGui::PopItemWidth();

        if (reclaim) {
            ImGui::SetKeyboardFocusHere(-1);
        }

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}