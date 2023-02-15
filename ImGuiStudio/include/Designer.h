#ifndef _IMGUI_STUDIO_DESIGNER
#define _IMGUI_STUDIO_DESIGNER

#include <GenericGuiStudio/include/Designer.h>
#include "Widgets.h"

namespace ImGuiStudio
{
    struct Designer
        : public GIDE::UI::Designer<float, float>
    {
        typedef GIDE::UI::Designer<float, float> Type;
        

        static Designer& Instance();

        using Type::toolbox;
        using Type::form;

        Widget& widget() const override;

        void step() override;
        static void Init();
        static bool Opened();
        static void Free();


    private:
        Designer();

    public:
        struct impl;
        impl* internal;
    };
}

#endif // _IMGUI_STUDIO_DESIGNER
