#pragma once

#include <cstdio>
#include <cstdlib>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace ui {
    class App {
    public:
        explicit App(int, char**);
    private:
        GLFWwindow* HandleGLFW();
    };
}
