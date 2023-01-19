#include <Designer.h>

#include <ImGui/imgui.h>

namespace ImGui
{
    void DrawBorder(ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness = 5.f, ImU32 col = IM_COL32(0, 130, 216, 255))
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
        if (ImGui::IsMouseDown(mouse_button))
        {
            *end_pos = ImGui::GetMousePos();
            ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
            draw_list->AddRect(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));   // Border
            draw_list->AddRectFilled(*start_pos, *end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));    // Background
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
            bool selection_in_progress;
            void Begin()
            {
                Window::Begin();

                selection_in_progress = false;

                if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                    selection_in_progress =
                        !ImGui::SelectionRect(&selection_start, &selection_end);
            }
        };
    }
}

struct ImGuiStudio::Designer::impl
{
    ImGuiStudio::Designer::Form::Component* active_component;

    ImGuiStudio::Widgets::DesignerWindow window;
};





ImGuiStudio::Designer::Designer()
    : internal(new impl())
{
    struct lambdas
    {
        static GIDE::System::Display::SizeType DisplaySize()
        {
            typedef GIDE::System::Display::SizeType result_type;

            result_type result = {
                result_type::Unit(ImGui::GetIO().DisplaySize.x),
                result_type::Unit(ImGui::GetIO().DisplaySize.y)
            };
            if (result.height < 10 || result.width < 10 || result.height > 9999 || result.width > 9999)
            {
                result.height = 720, result.width = 240;
            }
            return result;
        }
    };

    GIDE::RTTI::Override<
        static_cast<decltype(lambdas::DisplaySize)&>(GIDE::System::Display::Size)
    >(lambdas::DisplaySize);
}




ImGuiStudio::Designer& ImGuiStudio::Designer::Instance()
{
    static ImGuiStudio::Designer result;

    return result;
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
            for (auto component : components)
            {
                if (component->widget().x() > internal->window.selection_end.x && component->widget().y() > internal->window.selection_end.y)
                {
                    component->widget().click();
                }
            }
        }

        if (drag_delta.x || drag_delta.y)
        {

            bool moving_widgets = !mouse_moving;
            if (mouse_moving)
                for (auto component : components)
                {
                    if (component->widget().selected())
                    {
                        internal->active_component = component;
                        if (component->is_selected())
                        {
                            moving_widgets = true;
                            break;
                        }
                    }
                }
            if (moving_widgets)
                for (auto component : components)
                {
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
                Button::Begin();
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
            static std::map<std::string, std::vector<DesignerComponent>/**/> components;
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
