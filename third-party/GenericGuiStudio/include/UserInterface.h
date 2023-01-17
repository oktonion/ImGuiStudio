#ifndef _GIDE_UI_H
#define _GIDE_UI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "Designer.h"
#include "Properties.h"
#include "Widgets.h"

// std includes
#include <cstddef>

namespace GIDE
{
    namespace UI
    {
        template<class PosUnitT, class SizeUnitT>
        struct UI
        {
            typedef PosUnitT Position2DUnit;
            typedef SizeUnitT SizeUnit;

            typedef GIDE::UI::Designer<PosUnitT, SizeUnit>   Designer;
            typedef typename Designer::Toolbox               Toolbox;
            typedef GIDE::UI::Properties<PosUnitT, SizeUnit> Properties;

            struct Widgets
            {
                typedef GIDE::Implement::Widgets::IBasic<PosUnitT, SizeUnitT>       IBasic;
                typedef GIDE::Implement::Widgets::IWindow<PosUnitT, SizeUnitT>      IWindow;
                typedef GIDE::Implement::Widgets::ISubWindow<PosUnitT, SizeUnitT>   ISubWindow;
                typedef GIDE::Implement::Widgets::ICollapsible<PosUnitT, SizeUnitT> ICollapsible;
                typedef GIDE::Implement::Widgets::IButton<PosUnitT, SizeUnitT>      IButton;
                typedef GIDE::Implement::Widgets::ILabel<PosUnitT, SizeUnitT>       ILabel;

                template<class ValueT>
                using IEdit = GIDE::Implement::Widgets::IEdit<PosUnitT, SizeUnitT, ValueT>;

                
                typedef GIDE::Implement::Widgets::Basic<PosUnitT, SizeUnitT>       Basic;
                typedef GIDE::Implement::Widgets::Window<PosUnitT, SizeUnitT>      Window;
                typedef GIDE::Implement::Widgets::SubWindow<PosUnitT, SizeUnitT>   SubWindow;
                typedef GIDE::Implement::Widgets::Collapsible<PosUnitT, SizeUnitT> Collapsible;
                typedef GIDE::Implement::Widgets::Button<PosUnitT, SizeUnitT>      Button;
                typedef GIDE::Implement::Widgets::Label<PosUnitT, SizeUnitT>       Label;

                template<class ValueT>
                using Edit = GIDE::Implement::Widgets::Edit<PosUnitT, SizeUnitT, ValueT>;
            };
        };

    } namespace UserInterface = UI; // namespace UI



} // namespace GIDE

#endif // _GIDE_UI_H
