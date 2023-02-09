#include "./../include/ImGuiStudio.h"

#include <ImGui/misc/cpp/imgui_stdlib.h>

#include "Designer.h"
#include "Properties.h"
#include "Widgets.h"

#include <GenericGuiStudio/include/GIDE.h>

#include <vector>
#include <string>
#include <map>



namespace ImGuiStudio
{
    namespace Widgets
    {
        struct MainWindow;
    }

    static Widgets::Window& CreateWindow(Widgets::Window& parent);

    static Widgets::Collapsible& CreateCollapsible(Widgets::Window& parent);

    static Widgets::Button& CreateButton(Widgets::Window& parent);

    template<class ValueT>
    static Widgets::Edit<ValueT>& CreateEdit(Widgets::Window& parent);

    static Widgets::SubWindow& CreateSubWindow(Widgets::Window& parent);

    static Widgets::MainWindow& MainWindow();
}





bool ImGuiStudio::Begin(const ImVec2 &size, bool *is_open)
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    bool result = Begin(is_open);

    ImGui::SetWindowSize({ size.x, size.y });

    return result;
}


ImGuiStudio::UI::Designer& ImGuiStudio::Designer()
{
    return ImGuiStudio::Designer::Instance();
}

ImGuiStudio::UI::Properties& ImGuiStudio::Properties()
{
    return Properties::Instance();
}


namespace ImGuiStudio
{
    namespace Widgets
    {
        struct MainWindow
            : Window
        {
            bool hidden = false;

            void Begin()
            {
                if (hidden)
                    return;

                bool is_opened = true;


                ImGui::Begin("Dear ImGui Studio", &is_opened,
                    0
                    | ImGuiWindowFlags_NoBringToFrontOnFocus
                    | ImGuiWindowFlags_MenuBar
                    //| ImGuiWindowFlags_NoResize
                    //| ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse
                );


                if (!is_opened) { End(); hidden = true; return; }

                for (std::size_t i = 0; i < children.size(); ++i)
                {
                    children[i]->Begin();
                    children[i]->End();
                }
            }

            void End()
            {
                if (hidden) return;

                ImGui::End();
            }
        };
    }
}


namespace ImGuiStudio
{
    static std::size_t widget_id()
    {
        static std::size_t result = 0;
        return ++result;
    }

    static Widgets::Window& CreateWindow(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Window> windows;
        std::size_t id = widget_id();
        Widgets::Window& result = windows[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::Collapsible& CreateCollapsible(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Collapsible> collapsibles;
        std::size_t id = widget_id();
        Widgets::Collapsible& result = collapsibles[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::Button& CreateButton(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Button> buttons;
        std::size_t id = widget_id();
        Widgets::Button& result = buttons[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    template<class ValueT>
    static Widgets::Edit<ValueT>& CreateEdit(Widgets::Window& parent)
    {
        typedef Widgets::Edit<ValueT> Edit;
        static std::map<std::size_t, Edit> edits;
        std::size_t id = widget_id();
        Edit& result = edits[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::SubWindow& CreateSubWindow(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::SubWindow> subwindows;
        std::size_t id = widget_id();
        Widgets::SubWindow& result = subwindows[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::MainWindow& MainWindow() {
        static Widgets::MainWindow instance;
        return instance;
    }
}


void ImGuiStudio::Init()
{
    struct lambdas
    {
        static UI::Widgets::IWindow& CreateWindow()
        {
            return ImGuiStudio::CreateWindow(ImGuiStudio::MainWindow());
        }

        static UI::Widgets::ISubWindow& CreateSubWindow(UI::Widgets::IWindow& parent)
        {
            return ImGuiStudio::CreateSubWindow(dynamic_cast<ImGuiStudio::Widgets::Window&>(parent));
        }

        static UI::Widgets::IButton& CreateButton()
        {
            return ImGuiStudio::CreateButton(ImGuiStudio::MainWindow());
        }

        static UI::Widgets::ICollapsible& CreateCollapsible()
        {
            return ImGuiStudio::CreateCollapsible(ImGuiStudio::MainWindow());
        }

        static UI::Widgets::IEdit<float>& CreateFloatEdit()
        {
            return ImGuiStudio::CreateEdit<float>(ImGuiStudio::MainWindow());
        }
    };

    GIDE::RTTI::Override<GIDE::UI::Widgets::Window::Create>(lambdas::CreateWindow);
    GIDE::RTTI::Override<GIDE::UI::Widgets::SubWindow::Create>(lambdas::CreateSubWindow);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Button::Create>(lambdas::CreateButton);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Collapsible::Create>(lambdas::CreateCollapsible);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Edit<float>::Create>(lambdas::CreateFloatEdit);

    Designer::Init();
    Properties::Init();

    Designer().widget().parent(MainWindow());
}

bool ImGuiStudio::Begin(bool* is_open)
{
    bool result = ImGui::Begin("Dear ImGui Studio", is_open,
        0
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_MenuBar
        //| ImGuiWindowFlags_NoResize
        //| ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
    );

    MainWindow().Begin();

    ImGuiStudio::Properties::Begin();

    return result;
}

void ImGuiStudio::End()
{
    ImGuiStudio::Properties::End();

    MainWindow().End();

    ImGui::End();
}

