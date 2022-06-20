#include "./../include/ImGuiStudio.h"

#include <vector>
#include <string>

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

struct WidgetBasic
{
    std::string name;
    ImVec4 dim;
};

struct Widget
    : WidgetBasic
{
    void (*begin)(Widget&);
    void (*end)(Widget&);

    struct
    {
        void (*begin)(Widget&);
        void (*end)(Widget&);
    } props;

    Widget() :
        begin(NULL), end(NULL) 
    {
        props.begin = begin;
        props.end = end;
    }

    std::vector<Widget> child;
};

namespace
{
    std::vector<Widget> TopWindows;
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
        
        if (ImGui::CollapsingHeader("ToolBox"))
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + tool_box_shift * collapsing_header_lvl1);
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
            if (ImGui::CollapsingHeader("Windows"))
            {
                if (Widgets::Main::Button("Top Window"))
                {
                    Widget top_window;
                    struct lambdas
                    {
                        static void begin(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Windows::Begin(that.name.c_str());
                        }

                        static void end(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Windows::End();
                        }

                        static void begin_prop(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Text::BulletText("Test window prop");
                        }

                        static void end_prop(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            
                        }
                    };
                    top_window.begin = &lambdas::begin;
                    top_window.end = &lambdas::end;
                    top_window.props.begin = &lambdas::begin_prop;
                    top_window.props.end = &lambdas::end_prop;
                    top_window.name = "form";

                    TopWindows.push_back(top_window);
                } ImGui::Separator();

                if (Widgets::Main::Button("Child Window"))
                {

                } ImGui::Separator();
            }
            ImGui::EndGroup();
        }
        
        ImGui::EndChild();
    }

    ImGui::SameLine();

    if (ImGui::BeginTabBar("Forms"))
    {
        for (std::size_t i = 0; i < TopWindows.size(); ++i)
        {
            if (ImGui::BeginTabItem(TopWindows[i].name.c_str()))
            {
                ImGui::EndTabItem();
            }

        }
        ImGui::EndTabBar();
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