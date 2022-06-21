#include "./../include/ImGuiStudio.h"

#include <vector>
#include <string>

bool ImGuiStudio::Begin(const ImVec4 &dim, bool *is_open)
{
    bool result = Begin(is_open);

    ImGui::SetWindowSize({ dim.z, dim.w });
    ImGui::SetWindowPos({ dim.x, dim.y });

    return result;
}

bool ImGuiStudio::Begin(bool *is_open)
{
    return ImGui::Begin("Dear ImGui Studio", is_open,
        0
        |ImGuiWindowFlags_NoBringToFrontOnFocus
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

    std::string (*begin_to_string)(Widget&);
    std::string (*end_to_string)(Widget&);

    Widget() :
        begin(NULL), end(NULL), begin_to_string(NULL), end_to_string(NULL)
    {
        props.begin = begin;
        props.end = end;
    }

    void draw() {
        begin(*this);

        for (std::size_t i = 0; i < child.size(); ++i)
        {
            child[i].draw();
        }

        end(*this);
    }

    std::string to_string()
    {
        std::string result = begin_to_string(*this) + "\n";

        for (std::size_t i = 0; i < child.size(); ++i)
        {
            result += child[i].to_string() + "\n";
        }

        result += end_to_string(*this);

        return result;
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

                            Windows::BeginChild(that.name.c_str(), {that.dim.z, that.dim.w});

                            if (ImGui::BeginDragDropSource())
                            {
                                Widget* ptr = &that;
                                ImGui::SetDragDropPayload("DND_WIDGET", &ptr, sizeof(ptr));
                                ImGui::EndDragDropSource();
                            }

                            if (ImGui::BeginDragDropTarget())
                            {
                                ImGuiDragDropFlags target_flags = 0;
                                target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
                                //target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
                                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_WIDGET", target_flags))
                                {
                                    Widget* ptr = *(Widget**)payload->Data;
                                    that.child.push_back(*ptr);
                                }
                                ImGui::EndDragDropTarget();
                            }
                        }

                        static void end(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            ImGui::Button("awdawdawdawdaw");

                            Windows::EndChild();
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

                        static std::string begin_to_string(Widget& that)
                        {
                            return
                                "ImGui::Begin(" + that.name + ");";
                        }

                        static std::string end_to_string(Widget& that)
                        {
                            return
                                "ImGui::End();";
                        }
                    };
                    top_window.begin = &lambdas::begin;
                    top_window.end = &lambdas::end;
                    top_window.props.begin = &lambdas::begin_prop;
                    top_window.props.end = &lambdas::end_prop;
                    top_window.begin_to_string = &lambdas::begin_to_string;
                    top_window.end_to_string = &lambdas::end_to_string;

                    top_window.name = "form" + std::to_string(TopWindows.size());

                    TopWindows.push_back(top_window);
                } ImGui::Separator();

                Widget *dnd_widget = NULL;

                if (Widgets::Main::Button("Child Window"))
                {
                    dnd_widget = new Widget();
                    struct lambdas
                    {
                        static void begin(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Windows::BeginChild(that.name.c_str());

                            if (ImGui::BeginDragDropSource())
                            {
                                Widget* ptr = &that;
                                ImGui::SetDragDropPayload("DND_WIDGET", &ptr, sizeof(ptr));
                                ImGui::EndDragDropSource();
                            }

                            if (ImGui::BeginDragDropTarget())
                            {
                                ImGuiDragDropFlags target_flags = 0;
                                target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
                                //target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
                                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_WIDGET", target_flags))
                                {
                                    Widget* ptr = *(Widget**)payload->Data;
                                    that.child.push_back(*ptr);
                                }
                                ImGui::EndDragDropTarget();
                            }
                        }

                        static void end(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Windows::EndChild();
                        }

                        static void begin_prop(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;

                            Text::BulletText("Test child window prop");
                        }

                        static void end_prop(Widget& that)
                        {
                            using namespace ImGuiStudio::Widgets;
                        }

                        static std::string begin_to_string(Widget& that)
                        {
                            return
                                "ImGui::BeginChild(" + that.name + ");";
                        }

                        static std::string end_to_string(Widget& that)
                        {
                            return
                                "ImGui::EndChuld();";
                        }
                    };
                    dnd_widget->begin = &lambdas::begin;
                    dnd_widget->end = &lambdas::end;
                    dnd_widget->props.begin = &lambdas::begin_prop;
                    dnd_widget->props.end = &lambdas::end_prop;
                    dnd_widget->begin_to_string = &lambdas::begin_to_string;
                    dnd_widget->end_to_string = &lambdas::end_to_string;

                    dnd_widget->name = "child form" + std::to_string(TopWindows.size());

                } 
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("DND_WIDGET", &dnd_widget, sizeof(dnd_widget));
                    ImGui::EndDragDropSource();
                }
                ImGui::Separator();
            }
            ImGui::EndGroup();
        }
        
        ImGui::EndChild();
    }

    static std::vector<std::size_t> windows_to_remove;

    ImGui::SameLine();
    if (ImGui::BeginTabBar("##tabs", 
        0
        | ImGuiTabBarFlags_AutoSelectNewTabs 
        | ImGuiTabBarFlags_Reorderable 
        | ImGuiTabBarFlags_FittingPolicyResizeDown))
    {
        for (std::size_t i = 0; i < TopWindows.size(); ++i)
        {
            bool opened = true;
            
            if (ImGui::BeginTabItem(
                TopWindows[i].name.c_str(),
                &opened
            ))
            {
                TopWindows[i].draw();
                ImGui::EndTabItem();
            }

            if (!opened)
                windows_to_remove.push_back(i);
        }

        ImGui::EndTabBar();
    }

    for (std::size_t i = 0; i < windows_to_remove.size(); ++i)
    {
        std::size_t windex = windows_to_remove[i];
        if (!ImGui::IsPopupOpen("Main Window delete"))
            ImGui::OpenPopup("Main Window delete");
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Main Window delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextWrapped(("Are you sure you want to delete '" + TopWindows[windex].name + "' and all its content?").c_str());
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) 
            { 
                windows_to_remove.erase(windows_to_remove.begin() + i);
                TopWindows.erase(TopWindows.begin() + windex);
                ImGui::CloseCurrentPopup(); 
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) 
            {
                windows_to_remove.erase(windows_to_remove.begin() + i);
                ImGui::CloseCurrentPopup(); 
            }
            ImGui::EndPopup();
        }
    }


   
}

void ImGuiStudio::End(ImVec4& dim)
{

    dim.z = ImGui::GetWindowSize().x;
    dim.w = ImGui::GetWindowSize().y;




    ImGui::End();


}