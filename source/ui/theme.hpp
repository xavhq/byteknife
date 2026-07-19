#pragma once
/* maps token categories -> ImVec4 colors */

#include <imgui.h>

namespace ui {
    inline static void ApplyTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        /* shape: rounded, borderless, tighter spacing */
        style.WindowRounding    = 6.0f;
        style.ChildRounding     = 6.0f;
        style.FrameRounding     = 4.0f;
        style.PopupRounding     = 4.0f;
        style.ScrollbarRounding = 8.0f;
        style.GrabRounding      = 4.0f;
        style.TabRounding       = 4.0f;

        style.WindowBorderSize = 0.0f;
        style.ChildBorderSize  = 1.0f;
        style.FrameBorderSize  = 0.0f;
        style.PopupBorderSize  = 1.0f;

        style.WindowPadding   = ImVec2(12, 12);
        style.FramePadding    = ImVec2(8, 5);
        style.ItemSpacing      = ImVec2(8, 6);
        style.ItemInnerSpacing = ImVec2(6, 6);
        style.IndentSpacing    = 18.0f;
        style.ScrollbarSize    = 12.0f;
        style.GrabMinSize      = 10.0f;

        /* color palette: dark slate + single accent, not default ImGui gray/blue */
        colors[ImGuiCol_Text]                  = ImVec4(0.92f, 0.93f, 0.94f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.52f, 0.55f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.11f, 0.12f, 0.14f, 0.98f);
        colors[ImGuiCol_Border]                = ImVec4(0.20f, 0.21f, 0.24f, 1.00f);

        colors[ImGuiCol_FrameBg]               = ImVec4(0.15f, 0.16f, 0.19f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.21f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);

        colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.12f, 0.13f, 0.16f, 1.00f);

        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);

        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.25f, 0.27f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.32f, 0.35f, 0.40f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.38f, 0.42f, 0.48f, 1.00f);

        ImVec4 accent         = ImVec4(0.30f, 0.62f, 0.98f, 1.00f);
        ImVec4 accent_hover   = ImVec4(0.38f, 0.68f, 1.00f, 1.00f);
        ImVec4 accent_active  = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);

        colors[ImGuiCol_CheckMark]             = accent;
        colors[ImGuiCol_SliderGrab]            = accent;
        colors[ImGuiCol_SliderGrabActive]      = accent_active;
        colors[ImGuiCol_Button]                = ImVec4(0.17f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered]         = accent_hover;
        colors[ImGuiCol_ButtonActive]          = accent_active;
        colors[ImGuiCol_Header]                = ImVec4(0.17f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderHovered]         = accent_hover;
        colors[ImGuiCol_HeaderActive]          = accent_active;
        colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.21f, 0.24f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.21f, 0.24f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered]     = accent_hover;
        colors[ImGuiCol_ResizeGripActive]      = accent_active;

        colors[ImGuiCol_Tab]                   = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TabHovered]            = accent_hover;
        colors[ImGuiCol_TabActive]             = ImVec4(0.17f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.11f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.15f, 0.18f, 1.00f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.14f, 0.15f, 0.18f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.12f, 0.13f, 0.16f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.20f, 0.21f, 0.24f, 1.00f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);

        colors[ImGuiCol_PlotLines]             = accent;
        colors[ImGuiCol_PlotHistogram]         = accent;

        colors[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.35f);
        colors[ImGuiCol_DragDropTarget]        = accent;
    }
}
