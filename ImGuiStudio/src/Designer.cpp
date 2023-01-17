#include <Designer.h>

#include <ImGui/imgui.h>


struct ImGuiStudio::Designer::impl
{
    ImGuiStudio::Designer::Form::Component* active_component;
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



void ImGuiStudio::Designer::Begin()
{
    auto components = Instance().form().components();
    ImVec2 drag_delta;
    bool mouse_moving = false;

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
    if (drag_delta.x || drag_delta.y)
    {
        bool moving_widgets = !mouse_moving;
        if (mouse_moving)
            for (auto component : components)
            {
                if (component->widget().selected())
                {
                    Instance().internal->active_component = component;
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

void ImGuiStudio::Designer::End()
{
}

void ImGuiStudio::Designer::Step()
{
    Instance().step();
}



void ImGuiStudio::Designer::Init()
{
    Instance().init();
    Instance().widget().name("Designer");
    Instance().toolbox().widget().name("Toolbox");
}
