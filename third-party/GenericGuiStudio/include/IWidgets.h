#ifndef _GIDE_IWIDGETS_H
#define _GIDE_IWIDGETS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "GIDE.h"
#include "../src/internals.hpp"

// std includes
#include <cstddef>
#include <string>

namespace GIDE
{
    namespace Implement
    {
        namespace Widgets
        {
            template<class WidgetT>
            static WidgetT& Create()
            {
                System::Abort(
                    "GIDE internal error: " + RTTI::TypeInfo<WidgetT>::Name() + "::Create is not overridden: "
                    "make sure to call GIDE::Override<GIDE::Widgets::" + RTTI::TypeInfo<WidgetT>::Name() + "::Create>(MyCreateFunction)"
                );
                std::abort();
            }

            template<class WidgetT>
            static void Free(WidgetT&)
            {
                System::Print(
                    "GIDE internal: " + RTTI::TypeInfo<WidgetT>::Name() + "::Free is not overridden: "
                    "make sure to call GIDE::Override<GIDE::Widgets::" + RTTI::TypeInfo<WidgetT>::Name() + "::Free>(MyFreeFunction)"
                );
            }

            template<class PosUnitT, class SizeUnitT>
            struct IWidget
            {
                typedef GIDE::Size<SizeUnitT> Size;
                typedef GIDE::Position2D<PosUnitT> Position2D;

                typedef typename Size::Unit SizeUnit;
                typedef typename Position2D::Unit Position2DUnit;

                typedef IWidget Parent;

                // must have:

                // size
                virtual SizeUnit width()  const = 0;
                virtual SizeUnit height() const = 0;

                // position
                virtual Position2DUnit x() const = 0;
                virtual Position2DUnit y() const = 0;

                // state
                virtual bool hidden() const = 0;
                virtual bool selected() const = 0;
                virtual bool clicked() const = 0;


                // getters:

                // size
                virtual Size size() const
                {
                    Size result = { width(), height() };
                    return result;
                }

                // position
                virtual Position2D position() const
                {
                    Position2D result = { x(), y() };
                    return result;
                }

                // rect
                virtual Position2DUnit left() const {
                    return x();
                }
                virtual Position2DUnit right() const {
                    return x() + width();
                }
                virtual Position2DUnit top() const {
                    return y();
                }
                virtual Position2DUnit bottom() const {
                    return y() + height();
                }

                // name
                virtual std::string name() const { return ""; }

                // caption
                virtual std::string caption() const { return name(); }
            };

            template<class PosUnitT, class SizeUnitT>
            struct IBasic
                : IWidget<PosUnitT, SizeUnitT>
            {
                typedef IWidget<PosUnitT, SizeUnitT> Widget;
                typedef IBasic<PosUnitT, SizeUnitT> Parent;
                typedef typename Widget::Size Size;
                typedef typename Widget::Position2D Position2D;
                typedef typename Widget::SizeUnit SizeUnit;
                typedef typename Widget::Position2DUnit Position2DUnit;
                    
                // must have:

                static IBasic& Create()
                {
                    return detail::Global(Widgets::Create<IBasic>).Get<Create>()();
                }

                static void Free(IBasic& that)
                {
                    return detail::Global(Widgets::Free<IBasic>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }

                // Widget
                using Widget::width;
                using Widget::height;
                using Widget::x;
                using Widget::y;
                using Widget::hidden;
                using Widget::selected;

                // size
                virtual void width (const SizeUnit& value) = 0;
                virtual void height(const SizeUnit& value) = 0;

                // position
                virtual void x(const Position2DUnit& value) = 0;
                virtual void y(const Position2DUnit& value) = 0;

                // control
                virtual void hide() = 0;
                virtual void show() = 0;
                virtual void select(bool value = true) = 0;
                virtual void click(bool value = true) = 0;

                // setters

                // size
                virtual void size(const Size& value)
                {
                    width(value.width); height(value.height);
                }

                void size(const SizeUnit& value_width, const SizeUnit& value_height)
                {
                    width(value_width); height(value_height);
                }

                // position
                virtual void position(const Position2D& value)
                {
                    x(value.x); y(value.y);
                }

                void position(const Position2DUnit& value_x, const Position2DUnit& value_y)
                {
                    x(value_x); y(value_y);
                }

                // parent
                virtual void parent(Parent& value) = 0;

                // name
                virtual void name(const std::string& value) = 0;

                // caption
                virtual void caption(const std::string& value) { name(value); }

                // getters
                using Widget::size;
                using Widget::position;
                virtual Position2D global_position() const
                {
                    Position2D parent_global_position = {0., 0.};
                    if (parent())
                        parent_global_position = parent()->global_position();

                    Position2D result = { parent_global_position.x + x(), parent_global_position.y + y() };

                    return result;
                }

                // parent
                virtual Parent* parent() const { return NULL; }
                using Widget::name;
                using Widget::caption;

                virtual ~IBasic() {}
            };

            template<class PosUnitT, class SizeUnitT>
            struct IWindow
                : IBasic<PosUnitT, SizeUnitT>
            {
                typedef IBasic<PosUnitT, SizeUnitT> Widget;
                typedef typename Widget::Parent Parent;
                typedef typename Widget::Size Size;
                typedef typename Widget::Position2D Position2D;
                typedef typename Widget::SizeUnit SizeUnit;
                typedef typename Widget::Position2DUnit Position2DUnit;

                static IWindow& Create()
                {
                    return detail::Global(Widgets::Create<IWindow>).Get<Create>()();
                }

                static void Free(IWindow& that)
                {
                    return detail::Global(Widgets::Free<IWindow>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }

                // control
                virtual void place(Widget& widget)
                {
                    widget.parent(*this);
                }

                virtual void place(Widget& widget, const Position2D& pos)
                {
                    widget.parent(*this);
                    widget.position(pos);
                }
            };

            template<class PosUnitT, class SizeUnitT>
            struct ISubWindow
                : IWindow<PosUnitT, SizeUnitT>
            {
                static ISubWindow& Create(IWindow<PosUnitT, SizeUnitT> &parent)
                {
                    struct lambdas
                    {
                        typedef ISubWindow type;
                        static type& Create(IWindow<PosUnitT, SizeUnitT>&)
                        {
                            return GIDE::Implement::Widgets::Create<type>();
                        }
                    };
                    return detail::Global(lambdas::Create).Get<Create>()(parent);
                }

                static void Free(ISubWindow& that)
                {
                    return detail::Global(Widgets::Free<ISubWindow>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }
            };

            template<class PosUnitT, class SizeUnitT>
            struct ICollapsible
                : IBasic<PosUnitT, SizeUnitT>
            {

                // must have:

                static ICollapsible& Create()
                {
                    return detail::Global(Widgets::Create<ICollapsible>).Get<Create>()();
                }

                static void Free(ICollapsible& that)
                {
                    return detail::Global(Widgets::Free<ICollapsible>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }

                virtual void collapse(bool hide = true) = 0;
                virtual bool collapsed() const = 0;

                // control
                virtual void expand() {
                    collapse(false);
                }

                // state
                virtual bool expanded() const {
                    return !collapsed();
                }
            };

            template<class PosUnitT, class SizeUnitT>
            struct IButton
                : IBasic<PosUnitT, SizeUnitT>
            {

                // must have:

                static IButton& Create()
                {
                    return detail::Global(Widgets::Create<IButton>).Get<Create>()();
                }

                static void Free(IButton& that)
                {
                    return detail::Global(Widgets::Free<IButton>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }

                // control
                virtual void press() = 0;
                virtual void release() = 0;

                // state
                virtual bool pressed() const = 0;
                virtual bool released() const = 0;
            };

            template<class PosUnitT, class SizeUnitT, class ValueT>
            struct IEdit
                : IBasic<PosUnitT, SizeUnitT>
            {
                typedef ValueT  ValueType;

                static IEdit& Create()
                {
                    return detail::Global(Widgets::Create<IEdit>).Get<Create>()();
                }

                static void Free(IEdit& that)
                {
                    return detail::Global(Widgets::Free<IEdit>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }

                // must have:

                // getters
                virtual ValueType value() const = 0;

                // setters
                virtual void value(const ValueType& val) {
                    if (val != value())
                        on_value_changed(val);
                }

                // callbacks
                virtual void on_value_changed(const ValueType& val) {}
            };

            template<class PosUnitT, class SizeUnitT>
            struct ILabel
                : IBasic<PosUnitT, SizeUnitT>
            {

                // must have:

                static ILabel& Create()
                {
                    return detail::Global(Widgets::Create<ILabel>).Get<Create>()();
                }

                static void Free(ILabel& that)
                {
                    return detail::Global(Widgets::Free<ILabel>).Get<Free>()(that);
                }

                virtual void free()
                {
                    return Free(*this);
                }
            };
        } // namespace Widgets

    } // namespace Implement

    namespace RTTI
    {
        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::IWidget<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "IWidget"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::IBasic<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "IBasic"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::IButton<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "IButton"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::IWindow<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "IWindow"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::ISubWindow<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "ISubWindow"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::ILabel<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "ILabel"; }
        };
    } // namespace RTTI
} // namespace GIDE





#endif // _GIDE_IWIDGETS_H
