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

    static Widgets::Window& BorrowWindow(Widgets::Window& parent);

    static Widgets::Collapsible& BorrowCollapsible(Widgets::Window& parent);

    static Widgets::Button& BorrowButton(Widgets::Window& parent);

    template<class ValueT>
    static Widgets::Edit<ValueT>& BorrowEdit(Widgets::Window& parent);

    static Widgets::SubWindow& BorrowSubWindow(Widgets::Window& parent);

    namespace MainWindow
    {
        static Widgets::MainWindow& Instance();
    }
}

void ImGuiStudio::Step()
{
    Designer().step();
}

void ImGuiStudio::Free()
{
    ImGuiStudio::Designer::Free();
}

bool ImGuiStudio::MainWindow::Begin(const ImVec2 &size, bool *is_open)
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

    static Widgets::Window& BorrowWindow(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Window> windows;
        std::size_t id = widget_id();
        Widgets::Window& result = windows[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::Collapsible& BorrowCollapsible(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Collapsible> collapsibles;
        std::size_t id = widget_id();
        Widgets::Collapsible& result = collapsibles[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::Button& BorrowButton(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::Button> buttons;
        std::size_t id = widget_id();
        Widgets::Button& result = buttons[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    template<class ValueT>
    static Widgets::Edit<ValueT>& BorrowEdit(Widgets::Window& parent)
    {
        typedef Widgets::Edit<ValueT> Edit;
        static std::map<std::size_t, Edit> edits;
        std::size_t id = widget_id();
        Edit& result = edits[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    static Widgets::SubWindow& BorrowSubWindow(Widgets::Window& parent)
    {
        static std::map<std::size_t, Widgets::SubWindow> subwindows;
        std::size_t id = widget_id();
        Widgets::SubWindow& result = subwindows[id];
        result.id = id;
        result.parent(parent);
        return result;
    }

    namespace MainWindow
    {
        static Widgets::MainWindow& Instance() {
            static Widgets::MainWindow instance;
            return instance;
        }
    }
}


void ImGuiStudio::Init()
{
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

    {
        struct lambdas
        {
            static UI::Widgets::IWindow& BorrowWindow()
            {
                return ImGuiStudio::BorrowWindow(ImGuiStudio::MainWindow::Instance());
            }

            static UI::Widgets::ISubWindow& BorrowSubWindow(UI::Widgets::IWindow& parent)
            {
                return ImGuiStudio::BorrowSubWindow(dynamic_cast<ImGuiStudio::Widgets::Window&>(parent));
            }

            static UI::Widgets::IButton& BorrowButton()
            {
                return ImGuiStudio::BorrowButton(ImGuiStudio::MainWindow::Instance());
            }

            static UI::Widgets::ICollapsible& BorrowCollapsible()
            {
                return ImGuiStudio::BorrowCollapsible(ImGuiStudio::MainWindow::Instance());
            }

            static UI::Widgets::IEdit<float>& BorrowFloatEdit()
            {
                return ImGuiStudio::BorrowEdit<float>(ImGuiStudio::MainWindow::Instance());
            }
        };

        GIDE::RTTI::Override<GIDE::UI::Widgets::Window::Borrow>(lambdas::BorrowWindow);
        GIDE::RTTI::Override<GIDE::UI::Widgets::SubWindow::Borrow>(lambdas::BorrowSubWindow);
        GIDE::RTTI::Override<GIDE::UI::Widgets::Button::Borrow>(lambdas::BorrowButton);
        GIDE::RTTI::Override<GIDE::UI::Widgets::Collapsible::Borrow>(lambdas::BorrowCollapsible);
        GIDE::RTTI::Override<GIDE::UI::Widgets::Edit<float>::Borrow>(lambdas::BorrowFloatEdit);
    }

    Designer::Init();
    Properties::Init();

    Designer().widget().parent(MainWindow::Instance());
}

bool ImGuiStudio::MainWindow::Begin(bool* is_open)
{
    bool result = ImGui::Begin("Dear ImGui Studio", is_open,
        0
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_MenuBar
        //| ImGuiWindowFlags_NoResize
        //| ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
    );

    MainWindow::Instance().Begin();

    ImGuiStudio::Properties::Begin();

    return result;
}

void ImGuiStudio::MainWindow::End()
{
    ImGuiStudio::Properties::End();

    MainWindow::Instance().End();

    ImGui::End();
}

