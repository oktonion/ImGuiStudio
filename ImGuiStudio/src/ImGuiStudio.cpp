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

        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("Save"))
            {
                
            }

            if (ImGui::MenuItem("Open"))
            {
                
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    } 
}

void ImGuiStudio::End()
{
    ImGui::End();
}