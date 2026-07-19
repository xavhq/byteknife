#include <ui/app.hpp>
#include <ui/theme.hpp>

namespace ui {
    App::App(int argc, char** argv) {
        GLFWwindow* window = this->HandleGLFW();

        /* imgui setup */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(".assets/Satoshi-Variable.ttf", 16.0f);

        ui::ApplyTheme();

        //  ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /* -------------------- */
            ImGui::Begin("byteknife");
            ImGui::Text("Window + render chain is working.");
            ImGui::Text("GLFW, OpenGL3, and ImGui are all linked correctly.");
            ImGui::End();
            /* -------------------- */

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.10f, 0.10f, 0.12f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        /* clean up */
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
