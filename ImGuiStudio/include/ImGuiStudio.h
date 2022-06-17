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

    } // namespace Widgets

    bool Begin(const ImVec4& dim, bool* opened = NULL); // create Studio GUI main window
    void DrawInterface();
    void End(); // end Studio GUI main window

} // namespace ImGuiStudio


#endif // _IMGUI_STUDIO_H
