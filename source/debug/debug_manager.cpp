#include "debug_manager.hpp"

namespace QDUEngine
{
    void DebugManager::showSceneInformation(Scene *scene)
    {
        /*
        glUseProgram(m_lineShader.GetProgramID());
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        m_physicsWorldPtr->debugDrawWorld();
        */

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
}
