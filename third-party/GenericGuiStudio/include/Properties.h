#ifndef _GIDE_PROPERTIES_H
#define _GIDE_PROPERTIES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "GIDE.h"
#include "IWidgets.h"

// std includes
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace GIDE
{
    namespace Properties
    {
        enum Type
        {
            FloatingPoint = 1,
            Integer,
            Boolean,
            Position2D,
            Size,
            String
        };

        template<class WidgetT, Type>
        struct Traits { struct ValueType {}; };
        template<class WidgetT>
        struct Traits<WidgetT, FloatingPoint> { typedef float ValueType; };
        template<class WidgetT>
        struct Traits<WidgetT, Integer> { typedef int ValueType; };
        template<class WidgetT>
        struct Traits<WidgetT, Boolean> { typedef bool ValueType; };
        template<class WidgetT>
        struct Traits<WidgetT, Position2D> { typedef GIDE::Position2D<typename WidgetT::Position2DUnit> ValueType; };
        template<class WidgetT>
        struct Traits<WidgetT, Size> { typedef GIDE::Size<typename WidgetT::SizeUnit> ValueType; };
        template<class WidgetT>
        struct Traits<WidgetT, String> { typedef std::string ValueType; };

        template<class WidgetT, Type T = Type(0)>
        struct Property
            : Property<WidgetT, Type(0)>
        {
            typedef WidgetT WidgetType;
            typedef typename Traits<WidgetT, T>::ValueType ValueType;

            virtual void value(const ValueType&) = 0;
            virtual ValueType value() const = 0;

            virtual Type type() { return T; }
            virtual ValueType maximum() const { return ValueType(); }
            virtual ValueType minimum() const { return maximum(); }
        };
    }

    namespace UI
    {
        template<class PosUnitT, class SizeUnitT>
        struct Properties;

        template<>
        struct Properties<void, void> {
            struct Property {
                enum Tag { Borrow };
            };
        };

        template<class PosUnitT, class SizeUnitT>
        struct Properties
        {
            typedef PosUnitT Position2DUnit;
            typedef SizeUnitT SizeUnit;
            typedef Properties<PosUnitT, SizeUnitT> PropertiesType;
            typedef GIDE::Implement::Widgets::IWindow<PosUnitT, SizeUnitT> Widget;
            typedef GIDE::Properties::Property<PropertiesType> IProperty;
            typedef std::map<std::string, IProperty*> Group;

            template<GIDE::Properties::Type T>
            struct Property
                : GIDE::Properties::Property < PropertiesType, T>
                , Properties<void, void>::Property
            {
                typedef GIDE::Properties::Property<PropertiesType, T> Base;
                typedef typename Base::ValueType ValueType;
                typedef GIDE::Implement::Widgets::IEdit<PosUnitT, SizeUnitT, ValueType> Widget;
                typedef std::shared_ptr<Widget> WidgetPtr;

                struct WidgetDeleter {
                    void operator()(Widget* that) const
                    {
                        return that->free();
                    }
                };

                virtual Widget& widget() {
                    WidgetPtr& w_ptr = Widgets()[this->name()];
                    if (!w_ptr)
                        w_ptr.reset(&Widget::Borrow(), WidgetDeleter());
                    return *w_ptr;
                }

                void free() {
                    WidgetPtr& w_ptr = Widgets()[this->name()];
                    if (w_ptr)
                    {
                        Widget& w = *w_ptr;
                        if (&w == &widget() && w_ptr.unique())
                            w_ptr.reset();
                    }
                }

            protected:
                static std::map<std::string, WidgetPtr> &Widgets()
                {
                    
                    static std::map<std::string, WidgetPtr> widgets;

                    return widgets;
                }
            };

            virtual std::map<std::string, Group>& groups() {
                static std::map<std::string, Group>
                    result; return result;
            }
            virtual Widget& widget() { static Widget& result = Widget::Borrow(); return result; }

            void add(const std::string& group_name, IProperty& property)
            {
                if (groups()[group_name].count(property.name()))
                    GIDE::System::Abort("GIDE::UI::Properties internal error: property group '" + group_name + "' contains '" + property.name() + "' property", 0, GIDE::System::Log::Warning);
                groups()[group_name][property.name()] = &property;

                switch (property.type())
                {
                case GIDE::Properties::FloatingPoint:
                {
                    Property<GIDE::Properties::FloatingPoint>* float_property =
                        dynamic_cast<Property<GIDE::Properties::FloatingPoint>*>(&property);
                    if (float_property)
                    {
                        float_property->widget().parent(widget());
                        float_property->widget().caption(float_property->name());
                    }
                }
                }
            }

            void add(const std::string& group_name, std::vector<IProperty*>& properties)
            {
                for (std::size_t i = 0; i < properties.size(); ++i)
                {
                    add(group_name(*properties[i]));
                }
            }

            virtual void clear()
            {
                groups().clear();
            }

            Group& operator[](const std::string& group_name)
            {
                if (!groups().count(group_name))
                    GIDE::System::Print("GIDE::UI::Properties internal error: property group '" + group_name + "' not found");
                return groups()[group_name];
            }
        };
    }
    

    template<class WidgetT>
    struct Properties::Property<WidgetT, Properties::Type(0)>
    {
        typedef WidgetT WidgetType;

        virtual Type type() = 0;
        virtual std::string name() const = 0;
        virtual void free() = 0;

        template<Type T>
        void value(const typename Traits<WidgetType, T>::ValueType& val)
        {
            return dynamic_cast<Property<WidgetType, T>&>(*this).value(val);
        }

        template<Type T>
        typename Traits<WidgetType, T>::ValueType value() const
        {
            return dynamic_cast<const Property<WidgetType, T>&>(*this).value();
        }

        template<Type T>
        typename Traits<WidgetType, T>::ValueType maximum() const
        {
            return dynamic_cast<const Property<T>&>(*this).maximum();
        }

        template<Type T>
        typename Traits<WidgetType, T>::ValueType minimum() const
        {
            return dynamic_cast<const Property<WidgetType, T>&>(*this).minimum();
        }
    };


} // namespace GIDE


#endif // _GIDE_PROPERTIES_H
