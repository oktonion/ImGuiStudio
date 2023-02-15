#include <Designer.h>

#include <ImGui/imgui.h>

namespace ImGui
{
    ImColor GetSelectionColor()
    {
        return ImGui::GetStyle().Colors[ImGuiCol_Button];
    }

    void DrawBorder(ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness = 5.f, ImU32 col = GetColorU32(GetSelectionColor().Value))
    {
        ImVec2 vMin = { ImGui::GetWindowPos().x + obj_pos.x - distance_thickness, ImGui::GetWindowPos().y + obj_pos.y - distance_thickness };
        ImVec2 vMax = { vMin.x + obj_size.x + (distance_thickness * 2.f), vMin.y + obj_size.y + (distance_thickness * 2.f) };
        //ImGui::GetForegroundDrawList( )->AddRect( vMin, vMax, col ); //will draw on top of everything 
        ImGui::GetWindowDrawList()->AddRect(vMin, vMax, col, 0.f, 0, 0.01f);
    }

    bool SelectionRect(ImVec2* start_pos, ImVec2* end_pos, ImGuiMouseButton mouse_button = ImGuiMouseButton_Left)
    {
        IM_ASSERT(start_pos != NULL);
        IM_ASSERT(end_pos != NULL);

        if (ImGui::IsMouseClicked(mouse_button))
            *start_pos = ImGui::GetMousePos();
        if (ImGui::IsMouseDown(mouse_button) && ImGui::IsMouseDragging(mouse_button))
        {
            *end_pos = ImGui::GetMousePos();
            ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
            ImColor color = GetSelectionColor();
            auto R = 255 * color.Value.x; auto G = 255 * color.Value.y; auto B = 255 * color.Value.z; auto A = 255 * color.Value.z;
            draw_list->AddRect(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(R, G, B, 255)));   // Border
            draw_list->AddRectFilled(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(R, G, B, 50)));    // Background
        }
        return ImGui::IsMouseReleased(mouse_button);
    }
}


namespace ImGuiStudio
{
    namespace Widgets
    {
        struct DesignerWindow
            : Window
        {
            ImVec2 selection_start;
            ImVec2 selection_end;
            bool selection_in_progress, components_selected = false;
            void Begin()
            {
                Window::Begin();

                const bool can_select =
                    selection_in_progress ||
                    selection_start.x == selection_end.x;
                selection_in_progress = false;
                


                if (can_select &&
                    ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                {
                    auto window_pos = global_position();

                    selection_start.x += window_pos.x;
                    selection_start.y += window_pos.y;

                    selection_end.x += window_pos.x;
                    selection_end.y += window_pos.y;

                    {
                        auto window_left = window_pos.x, window_top = window_pos.y, window_right = window_left + width(), window_bottom = window_top + height();
                        if (ImGui::GetMousePos().x > window_left && ImGui::GetMousePos().y > window_top)
                            if (ImGui::GetMousePos().x < window_right && ImGui::GetMousePos().y < window_bottom)
                                selection_in_progress =
                                    !ImGui::SelectionRect(&selection_start, &selection_end);
                    }

                    selection_start.x -= window_pos.x;
                    selection_start.y -= window_pos.y;

                    selection_end.x -= window_pos.x;
                    selection_end.y -= window_pos.y;
                }

                else if (!components_selected)
                {
                    selection_start = {};
                    selection_end = selection_start;
                }
            }
        };
    }
}

struct ImGuiStudio::Designer::impl
{
    ImGuiStudio::Designer::Form::Component* active_component;

    ImGuiStudio::Widgets::DesignerWindow window;

    static std::unique_ptr<ImGuiStudio::Designer>& DesignerInstance()
    {
        static std::unique_ptr<ImGuiStudio::Designer> result(new ImGuiStudio::Designer());

        return result;
    }
};





ImGuiStudio::Designer::Designer()
    : internal(new impl())
{

}



ImGuiStudio::Designer& ImGuiStudio::Designer::Instance()
{
    return *ImGuiStudio::Designer::impl::DesignerInstance();
}

void ImGuiStudio::Designer::Free()
{
    ImGuiStudio::Designer::impl::DesignerInstance().reset();
}

bool ImGuiStudio::Designer::Opened()
{
    return !Instance().widget().hidden();
}

ImGuiStudio::Designer::Widget& ImGuiStudio::Designer::widget() const
{
    return internal->window;
}


void ImGuiStudio::Designer::step()
{
    auto components = form().components();
    ImVec2 drag_delta;
    bool mouse_moving = false;

    //if (widget().selected())
    {

        if (!ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            mouse_moving = (drag_delta.x || drag_delta.y);

            if (!mouse_moving)
            {
                if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
                    drag_delta.x += 1.f;
                if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
                    drag_delta.x -= 1.f;

                if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
                    drag_delta.y += 1.f;
                if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
                    drag_delta.y -= 1.f;
            }

            
        }


        if (internal->window.selection_in_progress)
        {
            internal->window.components_selected = false;
            for (auto component : components)
            {
                bool intersects = false;
                {
                    struct Rect
                    {
                        Position2DUnit left, right, top, bottom;
                    };
                    
                    auto wpos = component->widget().global_position();
                    auto window_pos = widget().global_position();
                    auto wsize = component->widget().size();

                    wpos.x -= window_pos.x; wpos.y -= window_pos.y;
                    Rect wrect{ wpos.x, wpos.x + wsize.width, wpos.y, wpos.y + wsize.height };
                    Rect selrect{ internal->window.selection_start.x, internal->window.selection_end.x, internal->window.selection_start.y, internal->window.selection_end.y };

                    if (selrect.left > selrect.right)
                        std::swap(selrect.left, selrect.right);
                    if (selrect.top > selrect.bottom)
                        std::swap(selrect.top, selrect.bottom);

                    intersects =
                        (wrect.left < selrect.right&& wrect.right > selrect.left &&
                            wrect.top < selrect.bottom&& wrect.bottom > selrect.top);


                   ImGui::Text("wrect: %f:%f:%f:%f\n selrect: %f:%f:%f:%f",
                       wrect.left,  wrect.top, wrect.right, wrect.bottom,
                       selrect.left,  selrect.top, selrect.right, selrect.bottom
                   );
                }

                if (intersects)
                    internal->window.components_selected = true;
                component->widget().select(intersects);
            }
        }

        else if (drag_delta.x || drag_delta.y)
        {

            bool moving_widgets = !mouse_moving;
            if (mouse_moving)
                for (auto component : components)
                {
                    if (component->widget().selected() && component->widget().clicked())
                    {
                        internal->active_component = component.get();
                        if (component->selected())
                        {
                            moving_widgets = true;
                            break;
                        }
                    }
                }
            if (moving_widgets)
                for (auto component : components)
                {
                    if (!component->selected())
                        continue;
                    auto pos = component->widget().position();
                    pos.x += drag_delta.x;
                    pos.y += drag_delta.y;
                    component->widget().position(pos);
                }
        }
    }

    toolbox().step(form());
}


namespace ImGuiStudio
{
    namespace Widgets
    {
        struct DesignerWidget
            : Button
        {
            void Begin()
            {
                {
                    auto pos = ImGui::GetCursorPos();
                    auto pos_w = position();
                    ImGui::SetCursorPosX(x()); ImGui::SetCursorPosY(y());
                    position(pos.x, pos.y);

                    auto label = caption(); if (label.empty()) label = name();
                    ImGui::Text(label.c_str());


                    size(ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);

                    const bool clicked = ImGui::IsItemClicked();
                    if (clicked)
                    click(clicked);
                }
                if (selected())
                    ImGui::DrawBorder({ x(), y() }, { width(), height() });
            }
        };
    }
    struct DesignerComponent
        : Designer::Form::Component
    {
        typedef Designer::Toolbox::Component ToolboxComponent;

        virtual Widget& widget() { return default_widget; }
        virtual const Widget& widget() const { return default_widget; }

        const ToolboxComponent& type() const { return *comp_type; }

        Widgets::DesignerWidget default_widget;
        const ToolboxComponent* comp_type;

        static DesignerComponent& Create(const ToolboxComponent& tbcomp)
        {
            static std::map<std::string, std::list<DesignerComponent>/**/> components;
            components[tbcomp.ID()].resize(components[tbcomp.ID()].size() + 1);
            DesignerComponent& result = components[tbcomp.ID()].back();
            result.comp_type = &tbcomp;
            result.default_widget.caption(tbcomp.name());
            return result;
        }
    };
}


void ImGuiStudio::Designer::Init()
{
    if (!ImGuiStudio::Designer::impl::DesignerInstance())
        ImGuiStudio::Designer::impl::DesignerInstance().reset(new ImGuiStudio::Designer());

    Instance().init();
    Instance().widget().name("Designer");
    Instance().toolbox().widget().name("Toolbox");

    struct lambdas
    {
        static Form::Component& Create(const Toolbox::Component& tbcomp)
        {
            return DesignerComponent::Create(tbcomp);
        }
    };

    GIDE::RTTI::Override<Form::Component::Create>(lambdas::Create);
}
