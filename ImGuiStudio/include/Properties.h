#ifndef _IMGUI_STUDIO_PROPERTIES
#define _IMGUI_STUDIO_PROPERTIES

#include <GenericGuiStudio/include/Properties.h>

namespace ImGuiStudio
{
    struct Properties
        : public GIDE::UI::Properties<float, float>
    {
        typedef GIDE::UI::Properties<float, float> Type;
        using typename Type::IProperty;

        static Properties& Instance();

        static void Init();
        static void Begin();
        static void End();
        static bool Opened();


    private:
        Properties();

        struct impl;
        impl* internal;
    };

} // namespace ImGuiStudio

#endif // _IMGUI_STUDIO_PROPERTIES
