#ifndef _GIDE_UI_H
#define _GIDE_UI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "Designer.h"
#include "Properties.h"

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

            typedef GIDE::UI::Designer<Position2DUnit, SizeUnit>   Designer;
            typedef typename Designer::Toolbox                     Toolbox;
            typedef GIDE::UI::Properties<Position2DUnit, SizeUnit> Properties;
        };

    } namespace UserInterface = UI; // namespace UI



} // namespace GIDE

#endif // _GIDE_UI_H
