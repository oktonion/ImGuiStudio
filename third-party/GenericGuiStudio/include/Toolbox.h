#ifndef _GIDE_TOOLBOX_H
#define _GIDE_TOOLBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GIDE includes
#include "GIDE.h"
#include "IWidgets.h"
#include "Properties.h"

// std includes
#include <cstddef>
#include <string>
#include <vector>
#include <list>

namespace GIDE
{
    struct Toolbox
    {
        struct Component
        {
            enum Type
            {
                Definition,
                Declaration
            };

            virtual std::string name() const = 0;
            virtual std::string group() const = 0;
            virtual std::string description() const = 0;
            virtual std::string ID() const {
                return group() + ":" + name();
            }
            virtual bool is_containter() const = 0;
            //virtual Image icon() const { return Image(); }

            typedef std::map<std::string, Properties::Type> Properties;
            virtual const Properties& properties() const = 0;

            virtual void serialize(std::vector<std::string>&, Type) const {}
            virtual void deserialize(const std::vector<std::string>&, Type) {}
        };

        virtual const std::list<const Component*>& components() const = 0;
        virtual Toolbox& add(const Component&) = 0;
        virtual Toolbox& remove(const Component&) = 0;

    };

} // namespace GIDE


#endif // _GIDE_TOOLBOX_H
