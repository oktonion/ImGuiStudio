#include <Properties.h>


#include <ImGuiStudio.h>


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

    auto components = ImGuiStudio::Designer().form().components();

    for (auto component : components)
    {
        if (component->is_selected())
        {

            Instance().clear();

            {
                for (auto property : component->type().properties())
                {
                    //Instance().add(property.first, property.second);

                    if (property.second == GIDE::Properties::FloatingPoint)
                    {
                        auto& prop = Instance().internal->fp_props[property.first];
                        prop.name_str = property.first;
                        Instance().add(component->type().group(), prop);
                    }
                }
            }

        }
    }
}

void ImGuiStudio::Properties::End()
{
}

bool ImGuiStudio::Properties::Opened()
{
    return !Instance().widget().hidden();
}