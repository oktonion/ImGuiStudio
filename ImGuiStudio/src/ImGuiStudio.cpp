#include "./../include/ImGuiStudio.h"

#include <ImGui/misc/cpp/imgui_stdlib.h>


#include <GenericGuiStudio/include/Widgets.h>

#include <vector>
#include <string>
#include <map>

namespace ImGui
{
    void DrawBorder(ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness = 5.f, ImU32 col = IM_COL32(255, 255, 0, 255))
    {
        ImVec2 vMin = { ImGui::GetWindowPos().x + obj_pos.x - distance_thickness, ImGui::GetWindowPos().y + obj_pos.y - distance_thickness };
        ImVec2 vMax = { vMin.x + obj_size.x + (distance_thickness * 2.f), vMin.y + obj_size.y + (distance_thickness * 2.f) };
        //ImGui::GetForegroundDrawList( )->AddRect( vMin, vMax, col ); //will draw on top of everything 
        ImGui::GetWindowDrawList()->AddRect(vMin, vMax, col, 0.f, 0, 0.01f);
    }
}



bool ImGuiStudio::Begin(const ImVec2 &size, bool *is_open)
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    bool result = Begin(is_open);

    ImGui::SetWindowSize({ size.x, size.y });

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

/*
struct WidgetBasic
{
    std::string name;
    ImVec4 dim;
};

struct Widget
    : WidgetBasic
{
    bool destroy;
    bool selected;

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
        destroy(false), selected(true)
        , begin(NULL), end(NULL), begin_to_string(NULL), end_to_string(NULL)
    {
        props.begin = begin;
        props.end = end;
    }

    void draw() {
        if (begin)
            begin(*this);
       
        for (std::size_t i = 0; i < child.size(); ++i)
        {
            if (child[i].destroy)
            {
                child.erase(child.begin() + i);
                destroy = false;
                continue;
            }

            child[i].draw();
        }

        if (end)
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
    std::pair<Widget, bool> CursorWidget;
    std::map<std::string, size_t> WidgetID;
    Widget *selected = NULL;

    void AddCursorWidget(Widget &that)
    {
        
        ImVec2 rel_mouse_pos = { ImGui::GetMousePos().x - ImGui::GetWindowPos().x, ImGui::GetMousePos().y - ImGui::GetWindowPos().y };
        if (CursorWidget.second)
        {
            ImGui::SetCursorPos({ rel_mouse_pos.x + 3.f, rel_mouse_pos.y + 3.f });
            if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                 CursorWidget.first.draw();
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                CursorWidget.second = false;
            }
        }

        if (CursorWidget.second)
        {
            static ImVec2 mouse_last_down_pos;
            static bool mouse_dragging = false;

            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                mouse_dragging = true;
                CursorWidget.first.dim.z = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).x;
                CursorWidget.first.dim.w = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).y;

                ImGui::SetCursorPos({ mouse_last_down_pos.x , mouse_last_down_pos.y });
                CursorWidget.first.draw();
            }
            else if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                mouse_last_down_pos = rel_mouse_pos;
                CursorWidget.first.dim.z = 0.f;
                CursorWidget.first.dim.w = 0.f;
            }
            else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                if (mouse_dragging)
                {
                    CursorWidget.first.dim.x = mouse_last_down_pos.x;
                    CursorWidget.first.dim.y = mouse_last_down_pos.y;
                    that.child.push_back(CursorWidget.first);
                    CursorWidget.second = false;
                    mouse_dragging = false;
                }
            }

        }
    }

    void AddSelectArea(Widget& that)
    {
        ImDrawList* window_draw_list = ImGui::GetWindowDrawList();
        //ImDrawList* background_draw_list = ImGui::GetBackgroundDrawList();

        static ImVec2 mouse_last_down_pos;
        static bool mouse_dragging = false;
        static ImVec2 mouse_last_up_pos;

        static ImVec2 selected_widgets_area[2];
        static bool selected_widgets_area_show = false;


        if (!ImGui::IsItemEdited() && 
            (mouse_last_down_pos.x < (ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 20.f)) &&
            (mouse_last_down_pos.y < (ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 20.f)) &&
            ImGui::IsMouseDragging(ImGuiMouseButton_Left)
            )
        {
            ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            mouse_last_up_pos.x = delta.x + mouse_last_down_pos.x;
            mouse_last_up_pos.y = delta.y + mouse_last_down_pos.y;
            window_draw_list->AddRectFilled(mouse_last_down_pos, mouse_last_up_pos, IM_COL32(255, 255, 255, 70));
            mouse_dragging = true;
        }
        else if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            selected_widgets_area_show = false;
            mouse_last_down_pos = ImGui::GetMousePos();
        }
        else if (mouse_dragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            for (std::size_t i = 0; i < that.child.size(); ++i)
            {
                bool intersects =
                    that.child[i].dim.x < mouse_last_up_pos.x - ImGui::GetWindowPos().x &&
                    that.child[i].dim.y + that.child[i].dim.w > mouse_last_down_pos.x - ImGui::GetWindowPos().x &&
                    that.child[i].dim.x + that.child[i].dim.z < mouse_last_up_pos.y - ImGui::GetWindowPos().y &&
                    that.child[i].dim.y > mouse_last_down_pos.y - ImGui::GetWindowPos().y;


                that.child[i].selected = intersects;
                if (intersects)
                {
                    selected_widgets_area[0].x = (selected_widgets_area[0].x < that.child[i].dim.x ? selected_widgets_area[0].x : that.child[i].dim.x - 1.f);
                    selected_widgets_area[0].y = (selected_widgets_area[0].y < that.child[i].dim.y ? selected_widgets_area[0].y : that.child[i].dim.y - 1.f);
                    selected_widgets_area[1].x = (selected_widgets_area[1].x > that.child[i].dim.z ? selected_widgets_area[1].x : that.child[i].dim.z + 1.f);
                    selected_widgets_area[1].y = (selected_widgets_area[1].y > that.child[i].dim.w ? selected_widgets_area[1].y : that.child[i].dim.w + 1.f);
                    selected_widgets_area_show = true;
                }
            }
        }

        if (selected_widgets_area_show)
        {
            window_draw_list->AddRect(selected_widgets_area[0], selected_widgets_area[1], IM_COL32(255, 255, 255, 70));
        }
    }

    void DrawMainProperties(Widget& that)
    {
        ImGui::BeginGroup();
        float value = that.dim.z;
        if (ImGui::SliderFloat("width", &value, 0.f, 9999.f)) {
            that.dim.z = value;
        }
        value = that.dim.w;
        if (ImGui::SliderFloat("height", &value, 0.f, 9999.f)) {
            that.dim.w = value;
        }
        ImGui::EndGroup();
    }


    void CreateWindow()
    {
        Widget top_window;
        struct lambdas
        {
            static void begin(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;


                Windows::Begin(that.name.c_str());

                that.dim.z = ImGui::GetWindowWidth();
                that.dim.w = ImGui::GetWindowHeight();

                selected = &that;

                {
                    ImVec2 pos = ImGui::GetCursorPos();
                    ImGui::Dummy(ImGui::GetWindowSize());
                    ImGui::SetCursorPos(pos);
                }

                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;
                    //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
                    //target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_WIDGET", target_flags))
                    {
                        Widget* ptr = (Widget*)payload->Data;
                        that.child.push_back(*ptr);
                    }
                    ImGui::EndDragDropTarget();
                }

                
                AddCursorWidget(that);
            }

            static void end(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                Windows::End();
            }

            static void begin_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                ImGui::BeginGroup();
                float value = that.dim.z;
                if (ImGui::SliderFloat("width", &value, 0.f, 9999.f)) {
                    that.dim.z = value;
                }
                value = that.dim.w;
                if (ImGui::SliderFloat("height", &value, 0.f, 9999.f)) {
                    that.dim.w = value;
                }
                
            }

            static void end_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                ImGui::EndGroup();
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
        top_window.dim.z = 420.f;
        top_window.dim.w = 380.f;

        TopWindows.push_back(top_window);
    }
    void CreateChild(Widget& dnd_widget, bool reset = true)
    {
        if (reset) dnd_widget = Widget();
        struct lambdas
        {
            static void begin(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                Windows::BeginChild(that.name.c_str());

                if (ImGui::IsItemFocused() || ImGui::IsItemClicked())
                    selected = &that;

                if (ImGui::BeginDragDropSource())
                {
                    Widget* ptr = &that;
                    ImGui::SetDragDropPayload("DND_WIDGET", &ptr, sizeof(ptr));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;
                    //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
                    //target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_WIDGET", target_flags))
                    {
                        Widget* ptr = (Widget*)payload->Data;
                        that.child.push_back(*ptr);
                    }
                    ImGui::EndDragDropTarget();
                }

                AddCursorWidget(that);
            }

            static void end(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                Windows::EndChild();
            }

            static void begin_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                DrawMainProperties(that);
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
                    "ImGui::EndChild();";
            }
        };
        dnd_widget.begin = &lambdas::begin;
        dnd_widget.end = &lambdas::end;
        dnd_widget.props.begin = &lambdas::begin_prop;
        dnd_widget.props.end = &lambdas::end_prop;
        dnd_widget.begin_to_string = &lambdas::begin_to_string;
        dnd_widget.end_to_string = &lambdas::end_to_string;

        dnd_widget.dim.z = 420.f;
        dnd_widget.dim.w = 380.f;
        dnd_widget.name = "child form" + std::to_string(WidgetID["child form"]++);

    }
    void CreateButton(Widget& dnd_widget, bool reset = true)
    {
        if (reset) dnd_widget = Widget();
        struct lambdas
        {
            static void begin(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                if (that.dim.x != 0.f || that.dim.y != 0.f)
                    ImGui::SetCursorPos({ that.dim.x, that.dim.y });
                Main::Button(that.name.c_str(), { that.dim.z, that.dim.w });

                if (ImGui::IsItemFocused())
                    selected = &that;
                
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("DND_WIDGET", &that, sizeof(that));
                    ImGui::EndDragDropSource();
                }
            }

            static void end(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

            }

            static void begin_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                DrawMainProperties(that);
            }

            static void end_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;
            }

            static std::string begin_to_string(Widget& that)
            {
                return
                    "ImGui::Button(" + that.name + ");";
            }

            static std::string end_to_string(Widget& that)
            {
                return
                    "";
            }
        };
        dnd_widget.begin = &lambdas::begin;
        dnd_widget.end = &lambdas::end;
        dnd_widget.props.begin = &lambdas::begin_prop;
        dnd_widget.props.end = &lambdas::end_prop;
        dnd_widget.begin_to_string = &lambdas::begin_to_string;
        dnd_widget.end_to_string = &lambdas::end_to_string;

        dnd_widget.name = "button" + std::to_string(WidgetID["button"]++);

    }
    void CreateText(Widget& dnd_widget, bool reset = true)
    {
        if (reset) dnd_widget = Widget();
        struct lambdas
        {
            static void begin(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                if (that.dim.x != 0.f || that.dim.y != 0.f)
                    ImGui::SetCursorPos({ that.dim.x, that.dim.y });

                if(that.dim.z > 0.f) 
                    ImGui::PushItemWidth(that.dim.z);

                Text::Text(that.name.c_str());

                if (ImGui::IsItemFocused() || ImGui::IsItemClicked())
                    selected = &that;
                
                if (that.dim.z > 0.f)
                    ImGui::PopItemWidth();

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload("DND_WIDGET", &that, sizeof(that));
                    ImGui::EndDragDropSource();
                }
            }

            static void end(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

            }

            static void begin_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;

                DrawMainProperties(that);
            }

            static void end_prop(Widget& that)
            {
                using namespace ImGuiStudio::Widgets;
            }

            static std::string begin_to_string(Widget& that)
            {
                return
                    "ImGui::Text(" + that.name + ");";
            }

            static std::string end_to_string(Widget& that)
            {
                return
                    "";
            }
        };
        dnd_widget.begin = &lambdas::begin;
        dnd_widget.end = &lambdas::end;
        dnd_widget.props.begin = &lambdas::begin_prop;
        dnd_widget.props.end = &lambdas::end_prop;
        dnd_widget.begin_to_string = &lambdas::begin_to_string;
        dnd_widget.end_to_string = &lambdas::end_to_string;

        dnd_widget.name = "text" + std::to_string(WidgetID["text"]++);

    }
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
            Widget dnd_widget;

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
                    CreateButton(CursorWidget.first);
                    CursorWidget.second = true;
                } 
                else if (ImGui::BeginDragDropSource())
                {
                    CreateButton(dnd_widget);
                    ImGui::SetDragDropPayload("DND_WIDGET", &dnd_widget, sizeof(dnd_widget));
                    ImGui::EndDragDropSource();
                }
                ImGui::Separator();

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
                    CreateWindow();
                } ImGui::Separator();

                if (Widgets::Main::Button("Child Window"))
                {
                    
                } 
                else if (ImGui::BeginDragDropSource())
                {
                    CreateChild(dnd_widget);
                    ImGui::SetDragDropPayload("DND_WIDGET", &dnd_widget, sizeof(dnd_widget));
                    ImGui::EndDragDropSource();
                } ImGui::Separator();
            }
            if (ImGui::CollapsingHeader("Text"))
            {
                if (Widgets::Main::Button("Simple Text"))
                {
                    CreateText(dnd_widget);
                }
                else if (ImGui::BeginDragDropSource())
                {
                    CreateText(dnd_widget);
                    ImGui::SetDragDropPayload("DND_WIDGET", &dnd_widget, sizeof(dnd_widget));
                    ImGui::EndDragDropSource();
                } ImGui::Separator();
            }
            ImGui::EndGroup();
        }
        
        
    } ImGui::EndChild();

    

    static std::vector<std::size_t> windows_to_remove;

    ImGui::SameLine();
    for (std::size_t i = 0; i < TopWindows.size(); ++i)
    {
        auto begin = TopWindows[i].begin;
        TopWindows[i].begin = NULL;
        begin(TopWindows[i]);
        
        if (!CursorWidget.second) AddSelectArea(TopWindows[i]);
        TopWindows[i].draw();

        TopWindows[i].begin = begin;

        if (TopWindows[i].destroy)
        {
            TopWindows[i].destroy = false;
            windows_to_remove.push_back(i);
        }
    }

    if (selected)
    {
        ImGui::SameLine();
        if (ImGui::Begin("Properties", NULL,
            0
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoResize
        ))
        {
            ImGui::Text(selected->name.c_str());
            ImGui::Dummy({ 200.f, 10.f });
            ImGui::BeginGroup();
            if (ImGui::CollapsingHeader("Main", ImGuiTreeNodeFlags_DefaultOpen))
            {
                selected->props.begin(*selected);
                selected->props.end(*selected);
            }
            ImGui::EndGroup();


        } ImGui::End();
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

    if (CursorWidget.second)
    {
        ImGui::SetCursorPos({ ImGui::GetMousePos().x + 3.f, ImGui::GetMousePos().y + 3.f });
        if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            CursorWidget.first.draw();
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            CursorWidget.second = false;
        }
    }
}
*/

void ImGuiStudio::End()
{
    ImGui::End();
}

struct MainWindow;

struct ImGuiStudio::Designer::impl
{
    struct IBasic;
    template<class T = Widgets::Basic>
    struct Basic;
    
    struct Window;
    struct SubWindow;
    struct Collapsible;
    struct Button;

    template<class ValueT>
    struct Edit;

    impl();

    Window& CreateWindow();
    SubWindow& CreateSubWindow(Window&);
    Collapsible& CreateCollapsible();
    Button& CreateButton();
    template<class ValueT>
    Edit<ValueT>& CreateEdit();

    ::MainWindow &top_window;
    typedef std::map<std::size_t, Window> windows_containter;
    windows_containter windows;
    std::map<std::size_t, Collapsible> collapsibles;
    std::map<std::size_t, Button> buttons;
    std::map<std::size_t, SubWindow> subwindows;

    ImGuiStudio::Designer::impl::IBasic* active_widget;
    ImGuiStudio::Designer::Form::Component* active_component;
};



struct ImGuiStudio::Designer::impl::IBasic
{
    static std::map<std::string, float>& EditFloatValues()
    {
        static std::map<std::string, float> values;
        return values;
    }

    static void EditFloatChanged(Widgets::IEditFloat& fedit, const Widgets::IEditFloat::ValueType &val)
    {
        EditFloatValues()[fedit.caption()] = val;
    }

    static std::map<std::string, int>& EditIntValues()
    {
        static std::map<std::string, int> values;
        return values;
    }

    static void EditIntChanged(Widgets::IEditInt& iedit, const Widgets::IEditInt::ValueType& val)
    {
        EditIntValues()[iedit.caption()] = val;
    }

    std::size_t id;

    typedef IBasic Child;
    typedef Widgets::IBasic::Parent Parent;

    virtual void AddChild(Child& value, std::size_t tab_order = 0) = 0;

    virtual void RemoveChild(Child& value) = 0;

    virtual void Begin() = 0;

    virtual void End() = 0;

    typedef ImGuiStudio::Properties::IProperty IProperty;
    std::vector<IProperty*> properties;

};

template<GIDE::Properties::Type T>
struct PropertyEdit;


template<class BaseT>
struct ImGuiStudio::Designer::impl::Basic
    : BaseT
    , IBasic
{

    typedef impl::IBasic::Child Child;
    typedef typename BaseT::Parent Parent;


    Basic()
    {
        struct FloatingPointProperty
            : ImGuiStudio::Properties::Property<GIDE::Properties::FloatingPoint>
        {
            typedef ImGuiStudio::Properties::Property<GIDE::Properties::FloatingPoint> Base;
            std::string prop_name;
            using typename Base::ValueType;
            ValueType prop_value;
            FloatingPointProperty(std::string name_) 
                : prop_name(name_) {
            }

            virtual ValueType value() const {
                return prop_value;
            }

            virtual void value(const ValueType& val) {
                prop_value = val;
            }

            virtual std::string name() const {
                return prop_name;
            }

            virtual Widget& widget() const 
            {
                static PropertyEdit<GIDE::Properties::FloatingPoint> edit;
                return edit;
            }
        };
        properties.push_back(new FloatingPointProperty("test"));
        properties.push_back(new FloatingPointProperty("test1"));
        properties.push_back(new FloatingPointProperty("test2"));
    }

    virtual void AddChild(Child& value, std::size_t tab_order = 0)
    {
        if (&value == this)
            throw("");
        Child* widget = dynamic_cast<Child*>(parent());
        if (widget) widget->AddChild(value, tab_order);
    }

    virtual void RemoveChild(Child& value)
    {
        if (&value == this)
            throw("");
        Child* widget = dynamic_cast<Child*>(parent());
        if (widget) widget->RemoveChild(value);
    }

    virtual void Begin()
    {

    }

    virtual void End()
    {

    }

    virtual void parent(Parent& value)
    {
        // remove from old parent
        {
            impl::IBasic* widget = dynamic_cast<impl::IBasic*>(parent());
            if (widget)
                widget->RemoveChild(*this);
        }

        // add to new parent
        {
            impl::IBasic* widget = dynamic_cast<impl::IBasic*>(&value);
            widget->AddChild(*this);
            BaseT::parent(value);
        }
    }



    using BaseT::parent;
};



struct ImGuiStudio::Designer::impl::Button
    : impl::Basic< Widgets::Button>
{
    virtual void Begin()
    {
        auto pos = ImGui::GetCursorPos();
        auto pos_w = position();
        ImGui::SetCursorPosX(x()); ImGui::SetCursorPosY(y());
        position(pos.x, pos.y);

        auto label = caption(); if (label.empty()) label = name();
        bool is_pressed = ImGui::Button(label.c_str());

        if (ImGui::IsItemActive()) 
            ImGuiStudio::Designer::Instance().internal->active_widget = this;
        
        if (is_pressed)
        {
            press();
        }
        else
        {
            release();
        }

        if(is_pressed)
            click();
        
        size(ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);

        if(selected())
            ImGui::DrawBorder({ pos_w.x, pos_w.y }, { width(), height() });
    }

    virtual void End()
    {

    }
};

template<>
struct ImGuiStudio::Designer::impl::Edit<float>
    : impl::Basic < Widgets::Edit<float> /**/>
{
    Edit()
    {
        set_on_value_changed_callback(impl::IBasic::EditFloatChanged);
    }

    virtual void Begin()
    {
        auto label = caption(); if (label.empty()) label = name();
        float fvalue = value();
        bool value_changed = ImGui::DragFloat(label.c_str(), &fvalue);
        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;
        if (value_changed)
        {
            value(fvalue);
        }
    }

    virtual void End()
    {

    }
};

template<>
struct ImGuiStudio::Designer::impl::Edit<int>
    : impl::Basic < Widgets::Edit<int> /**/>
{
    virtual void Begin()
    {
        auto label = caption(); if (label.empty()) label = name();
        int ivalue = value();
        if (ImGui::DragInt(label.c_str(), &ivalue))
        {
            value(ivalue);
        }

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;
    }

    virtual void End()
    {

    }
};

template<>
struct ImGuiStudio::Designer::impl::Edit<std::string>
    : impl::Basic < Widgets::Edit<std::string> /**/>
{
    virtual void Begin()
    {
        std::string svalue = value();
        if (ImGui::InputText(name().c_str(), &svalue))
            value(svalue);

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;
    }

    virtual void End()
    {

    }
};

struct ImGuiStudio::Designer::impl::Window
    : Basic <Widgets::Window>
{
    std::vector<Child*> children;

    void AddChild(Child& value, std::size_t tab_order = 0)
    {
        if (&value == this)
            throw("");
        children.push_back(&value);
    }

    void RemoveChild(Child& value)
    {
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i]->id == value.id)
                children.erase(children.begin() + i);
        }
    }

    void Begin()
    {
        if (hidden())
            return;

        bool is_opened = true;


        ImGui::Begin(name().c_str(), &is_opened,
            0
            //| ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_MenuBar
            //| ImGuiWindowFlags_NoResize
            //| ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;

        if (!is_opened) { End(); hide(); return; }

        for (std::size_t i = 0; i < children.size(); ++i)
        {
            children[i]->Begin();
            children[i]->End();
        }
    }

    void End()
    {
        if (hidden()) return;

        ImGui::End();
    }
};

struct ImGuiStudio::Designer::impl::SubWindow
    : Basic <Widgets::SubWindow>
{
    std::vector<Child*> children;

    void AddChild(Child& value, std::size_t tab_order = 0)
    {
        if (&value == this)
            throw("");
        children.push_back(&value);
    }

    void RemoveChild(Child& value)
    {
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i]->id == value.id)
                children.erase(children.begin() + i);
        }
    }

    void Begin()
    {
        if (hidden())
            return;
        
        bool is_opened = ImGui::BeginChild(("##form_" + name()).c_str(), {-1, -1}, true,
            0
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_MenuBar
            //| ImGuiWindowFlags_NoResize
            //| ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse);

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;

        if (!is_opened) { ImGui::EndChild(); hide(); return; }

        for (std::size_t i = 0; i < children.size(); ++i)
        {
            children[i]->Begin();
            children[i]->End();
        }
    }

    void End()
    {
        if (hidden()) return;

        ImGui::EndChild();
    }
};

struct MainWindow
    : ImGuiStudio::Designer::impl::Window
{
    MainWindow()
    {
        name("Dear ImGui Studio");
    }
    void Begin()
    {
        if (hidden())
            return;

        bool is_opened = true;


        ImGui::Begin(name().c_str(), &is_opened,
            0
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_MenuBar
            //| ImGuiWindowFlags_NoResize
            //| ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
        );

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;

        x(ImGui::GetWindowPos().x), y(ImGui::GetWindowPos().y);
        width(ImGui::GetWindowSize().x), height(ImGui::GetWindowSize().y);

        if (!is_opened) { End(); hide(); return; }

        for (std::size_t i = 0; i < children.size(); ++i)
        {
            children[i]->Begin();
            children[i]->End();
        }
    }

    void End()
    {
        if (hidden()) return;

        ImGui::End();
    }
};

struct IPropertyEdit
{
    std::vector<ImGuiStudio::Properties::IProperty*> props;
};

template<GIDE::Properties::Type T>
struct PropertyEdit
    : ImGuiStudio::Designer::impl::Edit<typename ImGuiStudio::Properties::Property<T>::ValueType >
    , IPropertyEdit
{
    typedef ImGuiStudio::Properties::Property<T> Properties;
    typedef typename Properties::ValueType ValueType;

    virtual ValueType value() const {
        ValueType result = ValueType();
        for (auto& prop : props)
            if (prop->value<T>() != result) result = prop->value<T>();
        return result;
    }

    virtual void value(const ValueType& val) {
        for (auto& prop : props)
            prop->value<T>(val);
    }
};


MainWindow& ImGuiStudio_MainWindow()
{
    static MainWindow result;

    return result;
}

void ImGuiStudio::MainWindow::Begin()
{
    ImGuiStudio_MainWindow().Begin();
}

bool ImGuiStudio::MainWindow::Opened()
{
    return !ImGuiStudio_MainWindow().hidden();
}

void ImGuiStudio::MainWindow::End()
{
    ImGuiStudio_MainWindow().End();
}

struct ImGuiStudio::Designer::impl::Collapsible
    : Basic<Widgets::Collapsible>
{
 
    std::vector<Child*> children;

    void AddChild(Child& value, std::size_t tab_order = 0)
    {
        if (&value == this)
            throw("");

        children.push_back(&value);
    }

    void RemoveChild(Child& value)
    {
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i]->id == value.id)
                children.erase(children.begin() + i);
        }
    }

    void Begin()
    {

        
        ImGui::BeginGroup();

        bool is_collapsed = 
            !ImGui::CollapsingHeader(name().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

        if (ImGui::IsItemActive())
            ImGuiStudio::Designer::Instance().internal->active_widget = this;

        if (is_collapsed != collapsed())
        {
            collapse(is_collapsed);
        }
            
        if (!is_collapsed)
        {
            for (std::size_t i = 0; i < children.size(); ++i)
            {
                children[i]->Begin();
                children[i]->End();
            }
        }
        ImGui::EndGroup();
        
    }


};

ImGuiStudio::Designer::impl::impl()
    : top_window(ImGuiStudio_MainWindow())
    , active_widget(0)
    , active_component(0)
{

}

//ImGuiStudio::Designer::Widgets::IWindow& ImGuiStudio::Designer::widget()
//{
//    Designer::Widgets::IWindow *test = internal->top_window;
//    return *internal->top_window;
//}


static std::size_t widget_id()
{
    static std::size_t result = 0;
    return ++result;
}

ImGuiStudio::Designer::impl::Window& ImGuiStudio::Designer::impl::CreateWindow()
{
    
    std::size_t id = widget_id();
    Window& result = windows[id];
    result.id = id;
    result.parent(top_window);
    return result;
}

ImGuiStudio::Designer::impl::Collapsible& ImGuiStudio::Designer::impl::CreateCollapsible()
{
    std::size_t id = widget_id();
    Collapsible& result = collapsibles[id];
    result.id = id;
    result.parent(top_window);
    return result;
}

ImGuiStudio::Designer::impl::Button& ImGuiStudio::Designer::impl::CreateButton()
{
    std::size_t id = widget_id();
    Button& result = buttons[id];
    result.id = id;
    result.parent(top_window);
    return result;
}

template<class ValueT>
ImGuiStudio::Designer::impl::Edit<ValueT>& ImGuiStudio::Designer::impl::CreateEdit()
{
    typedef Edit<ValueT> Edit;
    static std::map<std::size_t, Edit> edits;
    std::size_t id = widget_id();
    Edit& result = edits[id];
    result.id = id;
    result.parent(top_window);
    return result;
}

ImGuiStudio::Designer::impl::SubWindow& ImGuiStudio::Designer::impl::CreateSubWindow(Window& parent)
{
    std::size_t id = widget_id();
    SubWindow& result = subwindows[id];
    result.id = id;
    result.parent(parent);
    return result;
}

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




ImGuiStudio::Designer& ImGuiStudio_Designer()
{
    static ImGuiStudio::Designer result;
    
    return result;
}

ImGuiStudio::Designer& ImGuiStudio::Designer::Instance()
{
    return ImGuiStudio_Designer();
}

bool ImGuiStudio::Designer::Opened()
{
    return !ImGuiStudio_Designer().widget().hidden();
}



void ImGuiStudio::Designer::Begin()
{
    //ImGuiStudio_Designer().step();
    //ImGuiStudio_Designer().internal->top_window.Begin();
    //ImGuiStudio_Designer().internal->toolbox->Begin();

    auto components = ImGuiStudio_Designer().form().components();
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
            if (dynamic_cast<Widgets::IBasic*>(ImGuiStudio_Designer().internal->active_widget) == &component->widget())
            {
                ImGuiStudio_Designer().internal->active_component = component;
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
    //ImGuiStudio_Designer().internal->toolbox->End();
    //ImGuiStudio_Designer().internal->top_window.End();

    ImGuiStudio_Designer().internal->active_widget = 0;
}

void ImGuiStudio::Designer::Step()
{
    ImGuiStudio_Designer().step();
}



void ImGuiStudio::Designer::Init()
{
    struct lambdas 
    {
        static Widgets::IWindow &CreateWindow()
        {
            return ImGuiStudio_Designer().internal->CreateWindow();
        }

        static Widgets::ISubWindow& CreateSubWindow(Widgets::IWindow &parent)
        {
            return ImGuiStudio_Designer().internal->CreateSubWindow(dynamic_cast<ImGuiStudio::Designer::impl::Window&>(parent));
        }

        static Widgets::IButton &CreateButton()
        {
            return ImGuiStudio_Designer().internal->CreateButton();
        }

        static Widgets::ICollapsible &CreateCollapsible()
        {
            return ImGuiStudio_Designer().internal->CreateCollapsible();
        }

        static Widgets::IEditFloat& CreateFloatEdit()
        {
            return ImGuiStudio_Designer().internal->CreateEdit<float>();
        }
    };

    GIDE::RTTI::Override<GIDE::UI::Widgets::Window::Create>(lambdas::CreateWindow);
    GIDE::RTTI::Override<GIDE::UI::Widgets::SubWindow::Create>(lambdas::CreateSubWindow);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Button::Create>(lambdas::CreateButton);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Collapsible::Create>(lambdas::CreateCollapsible);
    GIDE::RTTI::Override<GIDE::UI::Widgets::Edit<float>::Create>(lambdas::CreateFloatEdit);

    ImGuiStudio_Designer().init();
    ImGuiStudio_Designer().widget().name("Designer");
    ImGuiStudio_Designer().toolbox().widget().name("Toolbox");
}

struct ImGuiStudio::Properties::impl
{
    ::MainWindow &top_window;

    impl() : top_window(ImGuiStudio_MainWindow()) {}

    struct FloatingPoint
        : Property<GIDE::Properties::FloatingPoint>
    {
        void value(const ValueType& v) {
            fp_value = v;
        }

        ValueType value() const {
            return fp_value;
        }

        std::string name() const {
            return name_str;
        }

        ValueType fp_value;
        std::string name_str;
    };

    std::map<std::string, FloatingPoint> fp_props;
};

ImGuiStudio::Properties::Properties()
    : internal(new impl())
{

}

ImGuiStudio::Properties& ImGuiStudio::Properties::Instance()
{
    static Properties result; return result;
}

void ImGuiStudio::Properties::Init()
{
    Instance().widget().name("Properties");
}

void ImGuiStudio::Properties::Begin()
{
    //Instance().widget().Begin();

    auto active_component = ImGuiStudio_Designer().internal->active_component;
    auto active_widget = ImGuiStudio_Designer().internal->active_widget;
    
    if (active_component)
    {

        Instance().clear();

        {
            for (auto property : active_component->type().properties())
            {
                //Instance().add(property.first, property.second);
                
                if (property.second == GIDE::Properties::FloatingPoint)
                {
                    auto& prop = Instance().internal->fp_props[property.first];
                    prop.name_str = property.first;
                    Instance().add(active_component->type().group(), prop);
                }
            }
        }

    }
}

void ImGuiStudio::Properties::End()
{
    //Instance().internal->top_window.End();
}

bool ImGuiStudio::Properties::Opened()
{
    return !Instance().widget().hidden();
}

