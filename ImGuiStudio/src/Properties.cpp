#include <Properties.h>

#include <ImGui/imgui.h>

template<GIDE::Properties::Type T>
struct Property
    : GIDE::UI::Properties<float, float>::Property<T>
{

};

struct ImGuiStudio::Properties::impl
{

    impl() {}

    struct FloatingPoint
        : Property<GIDE::Properties::FloatingPoint>
    {
        void value(const ValueType& v) {
            fp_value = v;
        }

        ValueType value() const {
            return fp_value;
        }

        std::string name() const {
            return name_str;
        }

        ValueType fp_value;
        std::string name_str;
    };

    std::map<std::string, FloatingPoint> fp_props;
};

ImGuiStudio::Properties::Properties()
    : internal(new impl())
{

}

ImGuiStudio::Properties& ImGuiStudio::Properties::Instance()
{
    static Properties result; return result;
}

void ImGuiStudio::Properties::Init()
{
    Instance().widget().name("Properties");
}

void ImGuiStudio::Properties::Begin()
{
    //Instance().widget().Begin();

    //auto active_component = ImGuiStudio_Designer().internal->active_component;
    //auto active_widget = ImGuiStudio_Designer().internal->active_widget;
    //
    //if (active_component)
    //{
    //
    //    Instance().clear();
    //
    //    {
    //        for (auto property : active_component->type().properties())
    //        {
    //            //Instance().add(property.first, property.second);
    //
    //            if (property.second == GIDE::Properties::FloatingPoint)
    //            {
    //                auto& prop = Instance().internal->fp_props[property.first];
    //                prop.name_str = property.first;
    //                Instance().add(active_component->type().group(), prop);
    //            }
    //        }
    //    }
    //
    //}
}

void ImGuiStudio::Properties::End()
{
    //Instance().internal->top_window.End();
}

bool ImGuiStudio::Properties::Opened()
{
    return !Instance().widget().hidden();
}