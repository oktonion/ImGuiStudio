#ifndef _IMGUI_STUDIO_DESIGNER
#define _IMGUI_STUDIO_DESIGNER

#include <GenericGuiStudio/include/Designer.h>

namespace ImGuiStudio
{
    struct Designer
        : public GIDE::UI::Designer<float, float>
    {
        typedef GIDE::UI::Designer<float, float> Type;
        

        static Designer& Instance();

        using Type::toolbox;
        using Type::form;

        static void Begin();
        static void End();
        static void Init();
        static void Step();
        static bool Opened();


    private:
        Designer();

    public:
        struct impl;
        impl* internal;
    };
}

#endif // _IMGUI_STUDIO_DESIGNER
