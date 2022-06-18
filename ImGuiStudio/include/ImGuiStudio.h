#ifndef _IMGUI_STUDIO_H
#define _IMGUI_STUDIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ImGui Studio includes
/*none*/

// third-party includes
#include <ImGui/imgui.h>

// std includes
/*none*/

namespace ImGuiStudio
{
    struct ID; // unique ID type for serialization purpose

    namespace Widgets
    {
        struct ID; // unique ID type for serialization purpose

        namespace Text
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::TextUnformatted;
            using ImGui::Text;
            using ImGui::TextColored;
            using ImGui::TextDisabled;
            using ImGui::TextWrapped;
            using ImGui::LabelText;
            using ImGui::BulletText;
        } // namespace Text

        namespace Main
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::Button;
            using ImGui::SmallButton;
            using ImGui::InvisibleButton;
            using ImGui::ArrowButton;
            using ImGui::Image;
            using ImGui::ImageButton;
            using ImGui::Checkbox;
            using ImGui::CheckboxFlags;
            using ImGui::CheckboxFlags;
            using ImGui::RadioButton;
            using ImGui::ProgressBar;
            using ImGui::Bullet;
        } // namespace Text

        namespace ComboBox
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::BeginCombo;
            using ImGui::EndCombo;
            using ImGui::Combo;
        } // namespace ComboBox

        namespace DragSliders
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::DragFloat;
            using ImGui::DragFloat2;
            using ImGui::DragFloat3;
            using ImGui::DragFloat4;
            using ImGui::DragFloatRange2;
            using ImGui::DragInt;
            using ImGui::DragInt2;
            using ImGui::DragInt3;
            using ImGui::DragInt4;
            using ImGui::DragIntRange2;
            using ImGui::DragScalar;
            using ImGui::DragScalarN;
        } // namespace DragSliders

        namespace Menus
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::BeginMenuBar;
            using ImGui::EndMenuBar;
            using ImGui::BeginMainMenuBar;
            using ImGui::EndMainMenuBar;
            using ImGui::BeginMenu;
            using ImGui::EndMenu;
            using ImGui::MenuItem;
        } // namespace Menus

    } // namespace Widgets

    bool Begin(const ImVec4& dim, bool* opened = NULL); // create Studio GUI main window
    void DrawInterface();
    void End(); // end Studio GUI main window

} // namespace ImGuiStudio


#endif // _IMGUI_STUDIO_H
