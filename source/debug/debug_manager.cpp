#include "debug_manager.hpp"
#include "examples/imgui_impl_glfw.cpp"
#include "examples/imgui_impl_opengl3.cpp"

namespace QDUEngine
{
    void DebugManager::end() noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    Vector3D DebugManager::getCameraPos()
    {
        return Vector3(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2]);
    }

    void DebugManager::showSceneInformation(Scene* scene)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Debug info:");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::InputFloat3("Camera position", m_cameraPos);
        ImGui::Separator();
        ImGui::Text("Scene data:");
        ImGui::TextUnformatted(scene->getData().dump(2).c_str());
        ImGui::SetNextWindowSize({0, 0});
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    void DebugManager::start()
    {
        glEnable(GL_DEBUG_OUTPUT);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }
}
