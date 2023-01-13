#ifndef _IMGUI_STUDIO_H
#define _IMGUI_STUDIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ImGui Studio includes
/*none*/

// third-party includes
#include <ImGui/imgui.h>
#include <GenericGuiStudio/include/Designer.h>
#include <GenericGuiStudio/include/Properties.h>
#include <GenericGuiStudio/include/GIDE.h>

// std includes
#include <map>

namespace ImGuiStudio
{
    struct ID; // unique ID type for serialization purpose

    namespace Widgets
    {
        struct ID; // unique ID type for serialization purpose

        namespace Windows
        {
            struct ID; // unique ID type for serialization purpose

            using ImGui::Begin;
            using ImGui::End;
            using ImGui::BeginChild;
            using ImGui::EndChild;
        } // namespace Windows

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

    bool Begin(const ImVec2& size, bool* opened = NULL); // create Studio GUI main window
    bool Begin(bool* opened = NULL); // create Studio GUI main window
    void DrawInterface();
    void End(); // end Studio GUI main window

    struct MainWindow
    {
        static void Begin();
        static bool Opened();
        static void End();
    };

    struct Designer
        : private GIDE::UI::Designer<float, float>
    {
        typedef GIDE::UI::Designer<float, float> Type;
        Designer();

        static Designer& Instance();

        using Type::toolbox;
        using Type::form;

        static void Begin();
        static void End();
        static void Init();
        static void Step();
        static bool Opened();


        struct impl;
        impl *internal;
    };

    struct Properties
        : private GIDE::UI::Properties<float, float>
    {
        typedef GIDE::UI::Properties<float, float> Type;
        using typename Type::IProperty;

        template<GIDE::Properties::Type T>
        struct Property
            : GIDE::UI::Properties<float, float>::Property<T>
        {

        };

        Properties();

        static Properties& Instance();

        static void Init();
        static void Begin();
        static void End();
        static bool Opened();


        struct impl;
        impl* internal;
    };

} // namespace ImGuiStudio



#endif // _IMGUI_STUDIO_H
