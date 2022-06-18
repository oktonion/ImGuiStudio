#include "./../include/ImGuiStudio.h"

bool ImGuiStudio::Begin(const ImVec4 &dim, bool *is_open)
{
    ImGui::SetNextWindowSize({ dim.z, dim.w });
    ImGui::SetNextWindowPos({ dim.x, dim.y });
    return ImGui::Begin("Dear ImGui Studio", is_open,
        ImGuiWindowFlags_NoBringToFrontOnFocus 
        | ImGuiWindowFlags_MenuBar 
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_MenuBar
    );
}

void ImGuiStudio::DrawInterface()
{
    if (ImGui::BeginMenuBar())
    {

        if (Widgets::Menus::BeginMenu("Project"))
        {
            if (Widgets::Menus::MenuItem("Save"))
            {
                
            }

            if (Widgets::Menus::MenuItem("Open"))
            {
                
            }

            Widgets::Menus::EndMenu();
        }
        Widgets::Menus::EndMenuBar();
    } 

    if (ImGui::BeginChild("Toolbox", { 200, 0 }))
    {

        if (ImGui::CollapsingHeader("ToolBox"))
        {
            if (ImGui::CollapsingHeader("Main"))
            {
                if (Widgets::Main::Button("ArrowButton"))
                {

                } ImGui::Separator();

                if (Widgets::Main::Button("Bullet"))
                {

                } ImGui::Separator();

                if (Widgets::Main::Button("Button"))
                {

                } ImGui::Separator();

                if (Widgets::Main::Button("Checkbox"))
                {

                } ImGui::Separator();
                
                if (Widgets::Main::Button("Checkbox Flags"))
                {

                } ImGui::Separator();
            }
        }
        ImGui::EndChild();
    }
}

void ImGuiStudio::End()
{
    ImGui::End();
}