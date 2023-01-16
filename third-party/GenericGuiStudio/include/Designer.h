#ifndef _GIDE_Designer_H
#define _GIDE_Designer_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "GIDE.h"
#include "Widgets.h"
#include "Toolbox.h"

#include "../src/internals.hpp"

// std includes
#include <cstddef>
#include <string>
#include <map>
#include <set>

namespace GIDE
{
    namespace UI
    {
        namespace Widgets
        {
            struct Basic       { enum Tag { Create }; };
            struct Collapsible { enum Tag { Create }; };
            struct Button      { enum Tag { Create }; };
            struct Window      { enum Tag { Create }; };
            struct SubWindow   { enum Tag { Create }; };

            template<class ValueT>
            struct Edit;
            template<>
            struct Edit<void> { enum Tag { Create }; };
            template<class ValueT>
            struct Edit : Edit<void> {};

            struct Label { enum Tag { Create }; };

            using Implement::Widgets::IBasic;
            using Implement::Widgets::ICollapsible;
            using Implement::Widgets::IButton;
            using Implement::Widgets::IWindow;
            using Implement::Widgets::ISubWindow;
            using Implement::Widgets::IEdit;
            using Implement::Widgets::ILabel;
        }


        template<class PosUnitT, class SizeUnitT>
        class Designer;

        template<>
        class Designer<void, void> {
        public:
            struct Form { 
                enum Tag { Create };
                struct Component {
                    enum Tag { Create };
                };
            };
        };

        template<class PosUnitT, class SizeUnitT>
        class Designer : public Designer<void, void>
        {
        public:

            typedef PosUnitT Position2DUnit;
            typedef SizeUnitT SizeUnit;

            struct Widgets
            {
                typedef GIDE::UI::Widgets::IBasic<Position2DUnit, SizeUnit> IBasic;
                typedef GIDE::UI::Widgets::ICollapsible<Position2DUnit, SizeUnit> ICollapsible;
                typedef GIDE::UI::Widgets::IButton<Position2DUnit, SizeUnit> IButton;
                typedef GIDE::UI::Widgets::IWindow<Position2DUnit, SizeUnit> IWindow;
                typedef GIDE::UI::Widgets::ISubWindow<Position2DUnit, SizeUnit> ISubWindow;
                typedef GIDE::UI::Widgets::ILabel<Position2DUnit, SizeUnit> ILabel;

                typedef GIDE::UI::Widgets::IEdit<Position2DUnit, SizeUnit, float> IEditFloat;
                typedef GIDE::UI::Widgets::IEdit<Position2DUnit, SizeUnit, int> IEditInt;

                

                struct Basic 
                    : GIDE::Implement::Widgets::Basic<Position2DUnit, SizeUnit>{};
                struct Collapsible
                    : GIDE::Implement::Widgets::Collapsible<Position2DUnit, SizeUnit> {};
                struct Button
                    : GIDE::Implement::Widgets::Button<Position2DUnit, SizeUnit> {};
                struct Window
                    : GIDE::Implement::Widgets::Window<Position2DUnit, SizeUnit> {};
                struct SubWindow
                    : GIDE::Implement::Widgets::SubWindow<Position2DUnit, SizeUnit> {};
                struct Label
                    : GIDE::Implement::Widgets::Label<Position2DUnit, SizeUnit> {};

                template<class ValueT>
                struct Edit : GIDE::Implement::Widgets::Edit<Position2DUnit, SizeUnit, ValueT> {};
            };

            typedef typename Widgets::IWindow Widget;

            virtual void init();
            virtual void step();

            struct Toolbox;

            virtual Toolbox& toolbox() {
                struct DesignerToolbox : Toolbox {
                    typename Toolbox::Widget& tbwidget;
                    DesignerToolbox() : tbwidget(Widgets::IWindow::Create()) {}
                    typename Toolbox::Widget& widget() const { return tbwidget; }
                };
                static DesignerToolbox result;
                return result;
            }

            virtual Widget& widget() const {
                static Widget& result = Widget::Create();
                return result;
            }

            struct Form;

            virtual Form& form() {
                
                static Form &result = CreateForm(widget());
                return result;
            }

        protected:

            // for Logging messages, default implementation uses GIDE::Abort and GIDE::Print
            virtual void log(const std::string& message, System::Log::Severity severity);

            static Form& CreateForm(const typename Toolbox::Component& toolbox_component)
            {
                Form& (*CreateFormFunc)(const typename Toolbox::Component&) = CreateForm;
                CreateFormFunc = detail::Global(CreateFormFunc).Get<CreateForm>();
                if (CreateFormFunc != CreateForm)
                    return CreateFormFunc(toolbox_component);

                System::Abort(
                    "GIDE internal error: GIDE::UI::Designer::Form::Create is not overridden: "
                    "make sure to call GIDE::Override<GIDE::UI::Designer::Form::Create>(MyCreateFunction)"
                );
                std::abort();
            }

            static Form& CreateForm(Widget &parent)
            {
                struct DesignerForm :
                    Form
                {
                    DesignerForm()
                        : default_widget(0) {}

                    Widget& widget() const{
                        return *default_widget;
                    }

                    Widget *default_widget;
                };

                static std::vector<Form*> detault_forms;
                
                DesignerForm* form = new DesignerForm();
                form->default_widget = &Form::Widget::Create(parent);
                form->widget().parent(parent);

                detault_forms.push_back(form);
                return *detault_forms.back();
            }
        };

        template<class PosUnitT, class SizeUnitT>
        struct Designer<PosUnitT, SizeUnitT>::Toolbox
            : GIDE::Toolbox
        {
            typedef typename Designer::Widgets::IWindow Widget;

            virtual Widget& widget() const = 0;

            virtual const std::list<const Component*>& components() const { return tbitems; }
            virtual Toolbox& add(const Component &comp) {
                std::string comp_ID = comp.ID();
                if (tbitems_map.count(comp_ID))
                    GIDE::System::Abort("cannot add '" + comp_ID + "' to designer toolbox: " + "component with this name is registered already");

                tbitems.push_back(&comp);
                tbitems_map[comp_ID] = &comp;

                if (!group_widgets.count(comp.group()))
                {
                    group_widgets[comp.group()] = &Designer::Widgets::Collapsible::Create();
                    widget().place(*group_widgets[comp.group()]);
                    group_widgets[comp.group()]->name(comp.group());
                    group_widgets[comp.group()]->caption(comp.group());
                }

                component_widgets[comp_ID] = &Designer::Widgets::Button::Create();
                component_widgets[comp_ID]->parent( *group_widgets[comp.group()]);
                component_widgets[comp_ID]->name(comp_ID);
                component_widgets[comp_ID]->caption(comp.name());
                
                return *this;
            }

            virtual Toolbox& remove(const Component& comp) {
                std::string comp_ID = comp.ID();
                if (!tbitems_map.count(comp_ID))
                    GIDE::System::Abort("cannot remove '" + comp_ID + "' from designer toolbox: " + "component with this name does not exist");

                for (auto value : tbitems)
                    if (value->ID() == comp_ID) { tbitems.remove(value); break; }
                tbitems_map.erase(comp_ID);
                return *this;
            }

            virtual void step(typename Designer<PosUnitT, SizeUnitT>::Form& form)
            {
                for (auto& comp_pair : component_widgets)
                {
                    if (comp_pair.second->pressed())
                    {
                        form.drop(*tbitems_map[comp_pair.first]);
                        break;
                    }
                }
            }

        private:
            std::list<const Component*> tbitems;
            std::map<std::string, const Component*> tbitems_map;
            std::map<std::string, typename Designer::Widgets::ICollapsible*> group_widgets;
            std::map<std::string, typename Designer::Widgets::IButton*>      component_widgets;
        };

        template<class PosUnitT, class SizeUnitT>
        struct Designer<PosUnitT, SizeUnitT>::Form
            : public Designer<void, void>::Form
        {
            typedef typename Toolbox::Component ToolboxComponent;
            typedef typename Designer::Widgets::ISubWindow Widget;
            typedef Designer<void, void> IDesigner;

            struct Component
                : public IDesigner::Form::Component
            {
                typedef typename Designer::Widgets::IBasic Widget;

                virtual bool is_container() const { return type().is_containter(); }
                virtual bool is_selected() const { return widget().clicked(); }
                virtual Widget& widget() const = 0;

                virtual const ToolboxComponent& type() const = 0;

                virtual ~Component() {}
            };

            typedef Designer<PosUnitT, SizeUnitT> Designer;

            virtual void drop(const ToolboxComponent& toolbox_component, Component& other_component)
            {
                Component& component = CreateComponent(toolbox_component);

                if (other_component.widget().parent())
                    component.widget().parent(*other_component.widget().parent());

                if (!component.widget().parent())
                {
                    GIDE::System::Print("cannot set parent on form '" + widget().name() + "' for '" + component.widget().name() + "' widget: reseting to form '" + widget().name() + "' as default");
                    component.widget().parent(widget());
                }

                if (!component.widget().parent())
                    GIDE::System::Abort("widget placing on form '" + widget().name() + "' for '" + component.widget().name() + "' failed: cannot set parent", GIDE::System::Log::Fatal);

                dropped_components.push_back(&component);
            }

            void drop(const ToolboxComponent& toolbox_component)
            {
                Component& component = CreateComponent(toolbox_component);

                component.widget().parent(widget());

                if (!component.widget().parent())
                    GIDE::System::Abort("widget placing on form '" + widget().name() + "' for '" + component.widget().name() + "' failed: cannot set parent", GIDE::System::Log::Fatal);

                dropped_components.push_back(&component);
            }

            void remove(const std::string &name)
            {
                for (typename std::list<Component*>::iterator it = dropped_components.begin(); dropped_components.end() != it; ++it)
                {
                    if ((*it)->name() == name)
                    {
                        delete (*it);
                        dropped_components.erase(it);
                        return;
                    }
                }

                GIDE::System::Print("component remove failed: cannot find '" + name + "' component on form '" + widget().name());
            }

            const std::list<Component*>& components() const { return dropped_components; }

            virtual Widget& widget() const = 0;

        protected:
            std::list<Component*> dropped_components;

            static Component& CreateComponentImpl(const ToolboxComponent& toolbox_component)
            {
                struct DefaultComponent
                    : Component
                {
                    virtual Widget& widget() const { return *default_widget; }

                    const ToolboxComponent& type() const { return *comp_type; }

                    Widget* default_widget;
                    const ToolboxComponent* comp_type;

                    ~DefaultComponent() { delete default_widget; }
                };
                typedef typename Designer::Widgets::IButton DefaultWidget;
                DefaultComponent* comp = new DefaultComponent();
                comp->comp_type = &toolbox_component;
                comp->default_widget = &DefaultWidget::Create();
                comp->default_widget->caption(comp->type().name());
                return *comp;
            }

            static Component& CreateComponent(const ToolboxComponent& toolbox_component)
            {
                return detail::Global(CreateComponentImpl).Get<CreateComponent>()(toolbox_component);
            }
        };

    }
} // namespace GIDE

namespace GIDE
{
    template<class PosUnitT, class SizeUnitT>
    void UI::Designer<PosUnitT, SizeUnitT>::log(const std::string& message, System::Log::Severity severity)
    {
        using namespace System;

        switch (severity)
        {
        case Log::Fatal:
        {
            Abort("Designer Fatal error: " + message);
            break;
        }
        case Log::Trace:
        {
            Print("Designer Trace: " + message);
            break;
        }
        case Log::Debug:
        {
            Print("Designer Debug: " + message);
            break;
        }
        case Log::Info:
        {
            Print("Designer Info: " + message);
            break;
        }
        case Log::Warning:
        {
            Print("Designer Warning: " + message);
            break;
        }
        case Log::Error:
        {
            Print("Designer Error: " + message);
            break;
        }
        }
    }

    template<class PosUnitT, class SizeUnitT>
    void UI::Designer<PosUnitT, SizeUnitT>::init()
    {
        widget().name("Designer");
        toolbox().widget().name("Toolbox");

        widget().width(System::Display::Size().width);
        widget().height(System::Display::Size().height);

        toolbox().widget().x(0), toolbox().widget().y(0);
        toolbox().widget().width(widget().width() / 4);
        toolbox().widget().height(widget().height() / 2);

        widget().x(toolbox().widget().width()), widget().y(0);

        widget().show();
        toolbox().widget().show();
        form().widget().show();
    }

    template<class PosUnitT, class SizeUnitT>
    void UI::Designer<PosUnitT, SizeUnitT>::step()
    {
        toolbox().step(form());
    }
} // namespace GIDE

namespace GIDE
{
    namespace RTTI
    {
        template<UI::Widgets::Basic::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::IBasic<PosUnitT, SizeUnitT>& (&value)())
        {
            typedef UI::Widgets::IBasic<PosUnitT, SizeUnitT> IBasic;
            detail::Global(IBasic::Create).Get<IBasic::Create>() = value;
        }

        template<UI::Widgets::Window::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::IWindow<PosUnitT, SizeUnitT>& (&value)())
        {
            typedef UI::Widgets::IWindow<PosUnitT, SizeUnitT> IWindow;
            detail::Global(IWindow::Create).Get<IWindow::Create>() = value;
        }

        template<UI::Widgets::SubWindow::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::ISubWindow<PosUnitT, SizeUnitT>& (&value)(UI::Widgets::IWindow<PosUnitT, SizeUnitT> &parent))
        {
            typedef UI::Widgets::ISubWindow<PosUnitT, SizeUnitT> ISubWindow;
            detail::Global(ISubWindow::Create).Get<ISubWindow::Create>() = value;
        }

        template<UI::Widgets::Button::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::IButton<PosUnitT, SizeUnitT>& (&value)())
        {
            typedef UI::Widgets::IButton<PosUnitT, SizeUnitT> IButton;
            detail::Global(IButton::Create).Get<IButton::Create>() = value;
        }

        template<UI::Widgets::Collapsible::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::ICollapsible<PosUnitT, SizeUnitT>& (&value)())
        {
            typedef UI::Widgets::ICollapsible<PosUnitT, SizeUnitT> ICollapsible;
            detail::Global(ICollapsible::Create).Get<ICollapsible::Create>() = value;
        }

        template<UI::Widgets::Label::Tag, class PosUnitT, class SizeUnitT>
        void Override(UI::Widgets::ILabel<PosUnitT, SizeUnitT>& (&value)())
        {
            typedef UI::Widgets::ILabel<PosUnitT, SizeUnitT> ILabel;
            detail::Global(ILabel::Create).Get<ILabel::Create>() = value;
        }

        template<UI::Widgets::Edit<void>::Tag, class PosUnitT, class SizeUnitT, class ValueT>
        void Override(UI::Widgets::IEdit<PosUnitT, SizeUnitT, ValueT>& (&value)())
        {
            typedef UI::Widgets::IEdit<PosUnitT, SizeUnitT, ValueT> IEdit;
            detail::Global(IEdit::Create).Get<IEdit::Create>() = value;
        }

        template<UI::Designer<void, void>::Form::Tag, class PosUnitT, class SizeUnitT>
        void Override(typename UI::Designer<PosUnitT, SizeUnitT>::Form& (&value)(const typename UI::Designer<PosUnitT, SizeUnitT>::Toolbox::Component&))
        {
            typedef typename UI::Designer<PosUnitT, SizeUnitT> Designer;
            detail::Global(Designer::CreateForm).Get<Designer::CreateForm>() = value;
        }

        template<UI::Designer<void, void>::Form::Component::Tag, class PosUnitT, class SizeUnitT>
        void Override(typename UI::Designer<PosUnitT, SizeUnitT>::Form::Component& (&value)(const typename UI::Designer<PosUnitT, SizeUnitT>::Toolbox::Component&))
        {
            typedef typename UI::Designer<PosUnitT, SizeUnitT>::Form DesignerForm;
            detail::Global(DesignerForm::CreateComponent).Get<DesignerForm::CreateComponent>() = value;
        }
    }

} // namespace GIDE

#endif // _GIDE_Designer_H
