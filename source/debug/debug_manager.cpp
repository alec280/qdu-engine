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

    void GLAPIENTRY MessageCallback(GLenum source,
                         GLenum type,
                         GLuint id,
                         GLenum severity,
                         GLsizei length,
                         const GLchar* message,
                         const void* userParam)
    {
        std::cout << "OpenGL Error: type = " << type << ", message = " << message << std::endl;
    }

    void DebugManager::showSceneInformation(Scene* scene)
    {
        //glUseProgram(m_lineShader.GetProgramID());
        //glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        //glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        //m_physicsWorldPtr->debugDrawWorld();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Debug Settings:");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("System Debug Draw:");
            ImGui::End();
        }
        //eventManager.Publish(DebugGUIEvent());
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    void DebugManager::start()
    {
        glEnable(GL_DEBUG_OUTPUT);
        //glDebugMessageCallback(MessageCallback, 0);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }
}
