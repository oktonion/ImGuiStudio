#ifndef _GGUIS_WIDGETS_REPO_H
#define _GGUIS_WIDGETS_REPO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GGUIS includes
#include "GGUIS.h"
#include "IWidgetsRepository.h"

// std includes
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <stdexcept>

namespace GGUIS
{
    template<class PosT = unsigned long, class SizeT = unsigned long>
    class WidgetsRepository
        : protected IWidgetsRepository
    {
    public:
        typedef IWidgetsRepository Base;

        struct Widget
            : Base::Widget<PosT, SizeT>
        {
            typedef Base::Widget<PosT, SizeT> Base;

            virtual const std::vector<Property<Base>*>& properties() {
                return props;
            }

        protected:
            std::vector<Property<Base>*> props;
        };

        typedef std::vector<WidgetInfo*> Toolset;

        virtual void register_toolset(Toolset& toolset)
        {
            for (std::size_t i = 0; i < toolset.size(); ++i)
            {
                toolbox().push_back(toolset[i]);
            }
        }

        virtual void place_widget(const std::string& id, Widget& widget)
        {
            if (widgets_map.count(id))
                throw(std::logic_error("index '" + id + "' is already placed"));
            std::size_t &indx = widgets_map[id];
            
            indx = widgets().size(); 
            widgets().push_back(&widget);
        }

        virtual Widget* pick_widget(const std::string& id)
        {
            if (!widgets_map.count(id))
                return 0;
            std::size_t& indx = widgets_map[id];

            Widget* result = widgets()[indx];
            widgets().erase(widgets().begin() + indx);
            widgets_map.erase(id);
            return result;
        }
    
        virtual std::vector<Widget*>& widgets() const
        {
            static std::vector<Widget*> result;
            return result;
        }

        virtual std::vector<WidgetInfo*>& toolbox() const
        {
            static std::vector<WidgetInfo*> result;
            return result;
        }

    protected:

        std::map<std::string, std::size_t> widgets_map;
    };

} // namespace GGUIS


#endif // _GGUIS_WIDGETS_REPO_H
