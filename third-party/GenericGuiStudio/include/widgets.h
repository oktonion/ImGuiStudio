#ifndef _GIDE_STATELESS_WIDGETS_H
#define _GIDE_STATELESS_WIDGETS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "GIDE.h"
#include "IWidgets.h"

// std includes
#include <cstddef>
#include <string>
#include <type_traits>

namespace GIDE 
{
    namespace Implement
    {
        namespace Widgets
        {

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT = IBasic<PosUnitT, SizeUnitT>
            >
            struct Basic
                : std::enable_if_t<
                    std::is_convertible_v<
                        InterfaceT*, IBasic<PosUnitT, SizeUnitT>*
                    >,
                    InterfaceT>
            {
                typedef InterfaceT Base;
                typedef typename Base::Parent Parent;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::Size Size;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;

                Basic();

                // getters

                // size
                virtual SizeUnit width() const;
                virtual SizeUnit height() const;

                // position
                virtual Position2DUnit x() const;
                virtual Position2DUnit y() const;

                // parent
                virtual Parent* parent() const;

                // name
                virtual std::string name() const;

                // caption
                virtual std::string caption() const;

                // setters

                // size
                virtual void width(const SizeUnit& value);
                virtual void height(const SizeUnit& value);

                // position
                virtual void x(const Position2DUnit& value);
                virtual void y(const Position2DUnit& value);

                // parent
                virtual void parent(Parent& value);

                // name
                virtual void name(const std::string& value);

                // caption
                virtual void caption(const std::string& value);

                // control
                virtual void hide();
                virtual void show();
                virtual void select(bool value = true);
                virtual void click(bool value = true);

                // state
                virtual bool hidden() const;
                virtual bool selected() const;
                virtual bool clicked() const;

            private:
                struct {
                    Position2D pos;
                    std::string name, caption;
                    Size size;
                    bool hidden, selected, clicked;
                    Parent* parent;
                } state;
            };

            template<class PosUnitT, class SizeUnitT>
            struct Window
                : Basic<PosUnitT, SizeUnitT, IWindow<PosUnitT, SizeUnitT>/**/>
            {
                typedef IWindow<PosUnitT, SizeUnitT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;

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
            struct SubWindow
                : Basic<PosUnitT, SizeUnitT, ISubWindow<PosUnitT, SizeUnitT>/**/>
            {
                typedef ISubWindow<PosUnitT, SizeUnitT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;

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
            struct Collapsible
                : Basic<PosUnitT, SizeUnitT, ICollapsible<PosUnitT, SizeUnitT>/**/>
            {
                typedef ICollapsible<PosUnitT, SizeUnitT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;

                Collapsible();

                // control
                virtual void collapse(bool hide = true);
                virtual void expand() {
                    collapse(false);
                }

                // state
                virtual bool collapsed() const;
                virtual bool expanded() const {
                    return !collapsed();
                }

            private:
                struct {
                    bool collapsed;
                } state;
            };

            template<class PosUnitT, class SizeUnitT>
            struct Button
                : Basic<PosUnitT, SizeUnitT, IButton<PosUnitT, SizeUnitT>/**/>
            {
                typedef IButton<PosUnitT, SizeUnitT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;

                Button();

                // control
                virtual void press();
                virtual void release();

                // state
                virtual bool pressed() const;
                virtual bool released() const;


            private:
                struct {
                    bool pressed;
                    bool released;
                } state;
            };

            template<class PosUnitT, class SizeUnitT, class ValueT>
            struct Edit
                : Basic<PosUnitT, SizeUnitT, IEdit<PosUnitT, SizeUnitT, ValueT>/**/>
            {
                typedef IEdit<PosUnitT, SizeUnitT, ValueT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;
                typedef ValueT  ValueType;

                Edit();

                // getters
                virtual ValueType value() const;

                // setters
                virtual void value(const ValueType& val);

                // callbacks
                virtual void on_value_changed(const ValueType& val);

            protected:
                typedef void (*on_value_changed_callback_type)(Base&, const ValueType&);
                struct {
                    ValueType value;
                    on_value_changed_callback_type on_value_changed_callback;
                } state;

            public:
                void set_on_value_changed_callback(on_value_changed_callback_type cb)
                { state.on_value_changed_callback = cb; }
            };

            template<class PosUnitT, class SizeUnitT>
            struct Label
                : Basic<PosUnitT, SizeUnitT, ILabel<PosUnitT, SizeUnitT>/**/>
            {
                typedef ILabel<PosUnitT, SizeUnitT> Base;
                typedef typename Base::Widget Widget;
                typedef typename Base::Parent Parent;
                typedef typename Base::Size Size;
                typedef typename Base::Position2D Position2D;
                typedef typename Base::SizeUnit SizeUnit;
                typedef typename Base::Position2DUnit Position2DUnit;
            };
        } // namespace Widgets
    } // namespace Implement
} // namespace GIDE

namespace GIDE
{
    namespace Implement
    {
        namespace Widgets
        {
            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            Basic<PosUnitT, SizeUnitT, InterfaceT>::Basic()
            {
                state.hidden = false;
                state.selected = false;
                state.clicked = false;
                state.parent = NULL;
                state.pos.x = 0;
                state.pos.y = 0;
                state.size.height = 20;
                state.size.width = 40;

                static std::size_t id = 0; ++id;
                char buf[32];
                std::snprintf(buf, sizeof(buf), "%d", id);
                name(RTTI::TypeInfo<InterfaceT>::Name() + std::string(&buf[0]));
            }

            template<class PosUnitT, class SizeUnitT>
            Button<PosUnitT, SizeUnitT>::Button()
            {
                state.pressed = false;
                state.released = true;
            }

            template<class PosUnitT, class SizeUnitT>
            Collapsible<PosUnitT, SizeUnitT>::Collapsible()
            {
                state.collapsed = false;
            }

            template<class PosUnitT, class SizeUnitT, class ValueT>
            Edit<PosUnitT, SizeUnitT, ValueT>::Edit()
            {
                state.value = ValueType();
            }


            // getters

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            typename Basic<PosUnitT, SizeUnitT, InterfaceT>::SizeUnit Basic<PosUnitT, SizeUnitT, InterfaceT>::width() const
            {
                return state.size.width;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            typename Basic<PosUnitT, SizeUnitT, InterfaceT>::SizeUnit Basic<PosUnitT, SizeUnitT, InterfaceT>::height() const
            {
                return state.size.height;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            typename Basic<PosUnitT, SizeUnitT, InterfaceT>::Position2DUnit Basic<PosUnitT, SizeUnitT, InterfaceT>::x() const
            {
                return state.pos.x;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            typename Basic<PosUnitT, SizeUnitT, InterfaceT>::Position2DUnit Basic<PosUnitT, SizeUnitT, InterfaceT>::y() const
            {
                return state.pos.y;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            typename Basic<PosUnitT, SizeUnitT, InterfaceT>::Parent* Basic<PosUnitT, SizeUnitT, InterfaceT>::parent() const
            {
                return state.parent;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            std::string Basic<PosUnitT, SizeUnitT, InterfaceT>::name() const
            {
                return state.name;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            std::string Basic<PosUnitT, SizeUnitT, InterfaceT>::caption() const
            {
                return state.caption;
            }

            template<class PosUnitT, class SizeUnitT, class ValueT>
            typename Edit<PosUnitT, SizeUnitT, ValueT>::ValueType
                Edit<PosUnitT, SizeUnitT, ValueT>::value() const
            {
                return state.value;
            }

            // setters

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::width(const SizeUnit& value)
            {
                state.size.width = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::height(const SizeUnit& value)
            {
                state.size.height = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::x(const Position2DUnit& value)
            {
                state.pos.x = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::y(const Position2DUnit& value)
            {
                state.pos.y = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::parent(Parent& value)
            {
                state.parent = &value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::name(const std::string& value)
            {
                state.name = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::caption(const std::string& value)
            {
                state.caption = value;
            }

            template<class PosUnitT, class SizeUnitT, class ValueT>
            inline void Edit<PosUnitT, SizeUnitT, ValueT>::value(const ValueType& val)
            {
                Base::value(val);
                state.value = val;
            }

            template<class PosUnitT, class SizeUnitT, class ValueT>
            inline void Edit<PosUnitT, SizeUnitT, ValueT>::on_value_changed(const ValueType& val)
            {
                if (state.on_value_changed_callback)
                {
                    on_value_changed_callback_type cb = state.on_value_changed_callback;
                    state.on_value_changed_callback = 0;
                    cb(*this, val);
                    state.on_value_changed_callback = cb;
                }
            }

            // control
            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::hide()
            {
                state.hidden = true;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::show()
            {
                state.hidden = false;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::select(bool value)
            {
                state.selected = value;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            void Basic<PosUnitT, SizeUnitT, InterfaceT>::click(bool value)
            {
                state.clicked = value;
            }

            template<class PosUnitT, class SizeUnitT>
            void Button<PosUnitT, SizeUnitT>::press()
            {
                state.pressed = true;
                state.released = false;
            }

            template<class PosUnitT, class SizeUnitT>
            void Button<PosUnitT, SizeUnitT>::release()
            {
                state.released = true;
                state.pressed = false;
            }

            template<class PosUnitT, class SizeUnitT>
            void Collapsible<PosUnitT, SizeUnitT>::collapse(bool value)
            {
                state.collapsed = value;
            }

            // state
            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            bool Basic<PosUnitT, SizeUnitT, InterfaceT>::hidden() const
            {
                return state.hidden;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            bool Basic<PosUnitT, SizeUnitT, InterfaceT>::selected() const
            {
                return state.selected;
            }

            template<class PosUnitT, class SizeUnitT,
                class InterfaceT>
            bool Basic<PosUnitT, SizeUnitT, InterfaceT>::clicked() const
            {
                return state.clicked;
            }

            template<class PosUnitT, class SizeUnitT>
            bool Button<PosUnitT, SizeUnitT>::pressed() const
            {
                return state.pressed;
            }

            template<class PosUnitT, class SizeUnitT>
            bool Button<PosUnitT, SizeUnitT>::released() const
            {
                return state.released;
            }

            template<class PosUnitT, class SizeUnitT>
            bool Collapsible<PosUnitT, SizeUnitT>::collapsed() const
            {
                return state.collapsed;
            }
        } // namespace Implement
    } // namespace Implement

    namespace RTTI
    {
        template<class PosUnitT, class SizeUnitT, class InterfaceT>
        struct TypeInfo<Implement::Widgets::Basic<PosUnitT, SizeUnitT, InterfaceT>/**/>
        {
            static std::string Name() { return "Basic"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::Button<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "Button"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::Window<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "Window"; }
        };

        template<class PosUnitT, class SizeUnitT>
        struct TypeInfo<Implement::Widgets::Label<PosUnitT, SizeUnitT>/**/>
        {
            static std::string Name() { return "Label"; }
        };
    } // namespace RTTI
} // namespace GIDE


#endif // _GIDE_STATELESS_WIDGETS_H
