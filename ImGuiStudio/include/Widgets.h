#ifndef _IMGUI_STUDIO_WIDGETS
#define _IMGUI_STUDIO_WIDGETS

// ImGui Studio includes
/*none*/

// third-party includes
#include <ImGui/imgui.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include <GenericGuiStudio/include/UserInterface.h>

// std includes
#include <string>
#include <map>

namespace ImGuiStudio
{
    typedef GIDE::UI::UI<float, float> UI;

    namespace Widgets
    {
        typedef UI::Widgets Widgets;

        struct IBasic;
        template<class T = Widgets::Basic>
        struct Basic;

        struct MainWindow;
        struct Window;
        struct SubWindow;
        struct Collapsible;
        struct Button;

        template<class ValueT>
        struct Edit;
    }
}

namespace ImGuiStudio
{
    namespace Widgets
    {
        struct IBasic
        {
            static std::map<std::string, float>& EditFloatValues()
            {
                static std::map<std::string, float> values;
                return values;
            }

            static void EditFloatChanged(Widgets::IEdit<float>& fedit, const Widgets::IEdit<float>::ValueType& val)
            {
                EditFloatValues()[fedit.caption()] = val;
            }

            static std::map<std::string, int>& EditIntValues()
            {
                static std::map<std::string, int> values;
                return values;
            }

            static void EditIntChanged(Widgets::IEdit<int>& iedit, const Widgets::IEdit<int>::ValueType& val)
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

            typedef UI::Properties::IProperty IProperty;
            std::vector<IProperty*> properties;

        };

        template<GIDE::Properties::Type T>
        struct PropertyEdit;

        template<class BaseT>
        struct Basic
            : BaseT
            , IBasic
        {

            typedef IBasic::Child Child;
            typedef typename BaseT::Parent Parent;


            Basic()
            {
                id = 0;
                struct FloatingPointProperty
                    : UI::Properties::Property<GIDE::Properties::FloatingPoint>
                {
                    typedef UI::Properties::Property<GIDE::Properties::FloatingPoint> Base;
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
                    IBasic* widget = dynamic_cast<IBasic*>(parent());
                    if (widget)
                        widget->RemoveChild(*this);
                }

                // add to new parent
                {
                    IBasic* widget = dynamic_cast<IBasic*>(&value);
                    widget->AddChild(*this);
                    BaseT::parent(value);
                }
            }



            using BaseT::parent;
        };

        struct Button
            : Basic< Widgets::Button>
        {
            virtual void Begin()
            {
                auto pos = ImGui::GetCursorPos();
                auto pos_w = position();
                ImGui::SetCursorPosX(x()); ImGui::SetCursorPosY(y());
                position(pos.x, pos.y);

                auto label = caption(); if (label.empty()) label = name();
                bool is_pressed = ImGui::Button(label.c_str());

                select(ImGui::IsItemActive());

                if (is_pressed)
                {
                    press();
                }
                else
                {
                    release();
                }

                if (is_pressed)
                    click();

                size(ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);

            }

            virtual void End()
            {

            }
        };

        template<>
        struct Edit<float>
            : Basic < Widgets::Edit<float> /**/>
        {
            Edit()
            {
                set_on_value_changed_callback(IBasic::EditFloatChanged);
            }

            virtual void Begin()
            {
                auto label = caption(); if (label.empty()) label = name();
                float fvalue = value();
                bool value_changed = ImGui::DragFloat(label.c_str(), &fvalue);
                select(ImGui::IsItemActive());
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
        struct Edit<int>
            : Basic < Widgets::Edit<int> /**/>
        {
            virtual void Begin()
            {
                auto label = caption(); if (label.empty()) label = name();
                int ivalue = value();
                if (ImGui::DragInt(label.c_str(), &ivalue))
                {
                    value(ivalue);
                }

                select(ImGui::IsItemActive());
            }

            virtual void End()
            {

            }
        };

        template<>
        struct Edit<std::string>
            : Basic < Widgets::Edit<std::string> /**/>
        {
            virtual void Begin()
            {
                std::string svalue = value();
                if (ImGui::InputText(name().c_str(), &svalue))
                    value(svalue);

                select(ImGui::IsItemActive());
            }

            virtual void End()
            {

            }
        };

        struct Window
            : Basic <Widgets::Window>
        {
            std::vector<Child*> children;

            struct
            {
                Position2D global_position;
            } state;

            Position2D global_position() const override
            {
                return state.global_position;
            }

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

                {
                    state.global_position = Basic <Widgets::Window>::global_position();
                    state.global_position.x += ImGui::GetWindowContentRegionMin().x;
                    state.global_position.y += ImGui::GetWindowContentRegionMin().y;
                }

                auto pos = ImGui::GetWindowPos();
                auto pos_w = position();
                //ImGui::SetCursorPosX(x()); ImGui::SetCursorPosY(y());
                position(pos.x, pos.y);

                width(ImGui::GetWindowWidth());
                height(ImGui::GetWindowHeight());

                select(ImGui::IsItemActive() || ImGui::IsWindowFocused());

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

        struct SubWindow
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

                bool is_opened = ImGui::BeginChild(("##form_" + name()).c_str(), { -1, -1 }, true,
                    0
                    | ImGuiWindowFlags_NoBringToFrontOnFocus
                    | ImGuiWindowFlags_MenuBar
                    //| ImGuiWindowFlags_NoResize
                    //| ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse);

                select(ImGui::IsItemActive());

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

        

        struct IPropertyEdit
        {
            std::vector<UI::Properties::IProperty*> props;
        };

        template<GIDE::Properties::Type T>
        struct PropertyEdit
            : Edit<typename UI::Properties::Property<T>::ValueType >
            , IPropertyEdit
        {
            typedef UI::Properties::Property<T> Properties;
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




        struct Collapsible
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

                select(ImGui::IsItemActive());

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
    }
}




#endif // _IMGUI_STUDIO_WIDGETS
