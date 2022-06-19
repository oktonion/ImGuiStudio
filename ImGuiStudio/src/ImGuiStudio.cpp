#include "./../include/ImGuiStudio.h"

bool ImGuiStudio::Begin(const ImVec4 &dim, bool *is_open)
{
    bool result = ImGui::Begin("Dear ImGui Studio", is_open,
        ImGuiWindowFlags_NoBringToFrontOnFocus 
        | ImGuiWindowFlags_MenuBar 
        //| ImGuiWindowFlags_NoResize
        //| ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
    );

    ImGui::SetWindowSize({ dim.z, dim.w });
    ImGui::SetWindowPos({ dim.x, dim.y });

    return result;
}

bool ImGuiStudio::Begin(bool *is_open)
{
    return ImGui::Begin("Dear ImGui Studio", is_open,
        ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_MenuBar
        //| ImGuiWindowFlags_NoResize
        //| ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
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
        const float tool_box_shift = 10.f;
        enum {
            collapsing_header_lvl1 = 1, 
            collapsing_header_lvl2,
            collapsing_header_lvl3
        };
        
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + tool_box_shift * collapsing_header_lvl1);
        if (ImGui::CollapsingHeader("ToolBox"))
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + tool_box_shift * collapsing_header_lvl2);
            ImGui::BeginGroup();
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
            ImGui::EndGroup();
        }
        
        ImGui::EndChild();
    }
}

void ImGuiStudio::End(ImVec4& dim)
{

    dim.z = ImGui::GetWindowSize().x;
    dim.w = ImGui::GetWindowSize().y;
    dim.x = ImGui::GetWindowPos().x;
    dim.y = ImGui::GetWindowPos().y;

    ImGui::SetWindowPos({ 0.f, 0.f });

    ImGui::End();
}