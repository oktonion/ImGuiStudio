#include "./../include/GIDE.h"
#include "./../include/Designer.h"

#include <iostream>

namespace GIDE
{
    using namespace detail;

    namespace System
    {
        void Abort(const std::string& message, int err, Log::Severity severity)
        {
            struct lambdas {
                static void Call(const std::string& message, int err, GIDE::System::Log::Severity severity)
                {
                    static
                    const char* severity_cstr[] = {
                        0, 0, 0, 0, 0, 0
                    }; 
                    severity_cstr[GIDE::System::Log::Trace]    = "Trace";
                    severity_cstr[GIDE::System::Log::Debug]    = "Debug";
                    severity_cstr[GIDE::System::Log::Info]     = "Info";
                    severity_cstr[GIDE::System::Log::Warning]  = "Warning";
                    severity_cstr[GIDE::System::Log::Error]    = "Error";
                    severity_cstr[GIDE::System::Log::Fatal]    = "Fatal";
                    enum { severity_cstr_size = sizeof(severity_cstr) / sizeof(severity_cstr[0]) };

                    std::string prefix = "GIDE "; 
                    prefix += (severity < severity_cstr_size ? severity_cstr[severity] : "");
                    Print(prefix + ":" + message);

                    if (ImplementationReady())
                        std::abort();
                }
            };
            return Global(lambdas::Call).Get<Abort>()(message, err, severity);
        }

        void Print(const std::string& message)
        {
            struct lambdas {
                static void Call(const std::string& message)
                {
                    std::cout << message << std::endl;
                }
            };
            return Global(lambdas::Call).Get<Print>()(message);
        }

        bool ImplementationReady()
        {
            struct lambdas {
                static bool Call() {
                    return true;
                }
            };
            return Global(lambdas::Call).Get<ImplementationReady>()();
        }

        Display::SizeType Display::Size() {
            if (&Display::Size == Global(Display::Size).Get<Display::Size>())
                Abort("Cannot get current display size");
            return Global(Display::Size).Get<Display::Size>()();
        }
    }

    namespace RTTI
    {
        template<>
        void Override<System::Abort>(void(&value)(const std::string&, int, System::Log::Severity))
        {
            Global(System::Abort).Get<System::Abort>() = value;
        }


        template<>
        void Override<System::Display::Size>(System::Display::SizeType(&value)())
        {
            Global(System::Display::Size).Get<System::Display::Size>() = value;
        }

        template<>
        void Override<System::ImplementationReady>(bool(&value)())
        {
            Global(System::ImplementationReady).Get<System::ImplementationReady>() = value;
        }

        template<>
        void Override<System::Print>(void(&value)(const std::string&))
        {
            Global(System::Print).Get<System::Print>() = value;
        }
    }

    namespace RTTI
    {
        template<>
        std::string TypeInfo<GIDE::UI::Widgets::Basic>::Name()
        {
            struct lambdas {
                static std::string Call() { return "Basic"; }
            };

            return Global(lambdas::Call).Get<TypeInfo::Name>()();
        }

        template<>
        std::string TypeInfo<GIDE::UI::Widgets::Window>::Name()
        {
            struct lambdas {
                static std::string Call() { return "Window"; }
            };

            return Global(lambdas::Call).Get<TypeInfo::Name>()();
        }

        template<>
        std::string TypeInfo<GIDE::UI::Widgets::Button>::Name()
        {
            struct lambdas {
                static std::string Call() { return "Button"; }
            };

            return Global(lambdas::Call).Get<TypeInfo::Name>()();
        }

        template<>
        std::string TypeInfo<GIDE::UI::Widgets::Collapsible>::Name()
        {
            struct lambdas {
                static std::string Call() { return "Collapsible"; }
            };

            return Global(lambdas::Call).Get<TypeInfo::Name>()();
        }

        template<>
        std::string TypeInfo<GIDE::UI::Widgets::Label>::Name()
        {
            struct lambdas {
                static std::string Call() { return "Label"; }
            };

            return Global(lambdas::Call).Get<TypeInfo::Name>()();
        }
    }

    namespace RTTI
    {
        template<>
        void Override<TypeInfo<int>::Name>(std::string(&value)())
        {
            Global(TypeInfo<int>::Name).Get<TypeInfo<int>::Name>() = value;
        }

        template<>
        void Override<TypeInfo<GIDE::UI::Widgets::Basic>::Name>(std::string(&value)())
        {
            Global(TypeInfo<GIDE::UI::Widgets::Basic>::Name).Get<TypeInfo<GIDE::UI::Widgets::Basic>::Name>() = value;
        }

        template<>
        void Override<TypeInfo<GIDE::UI::Widgets::Window>::Name>(std::string(&value)())
        {
            Global(TypeInfo<GIDE::UI::Widgets::Window>::Name).Get<TypeInfo<GIDE::UI::Widgets::Window>::Name>() = value;
        }

        template<>
        void Override<TypeInfo<GIDE::UI::Widgets::Button>::Name>(std::string(&value)())
        {
            Global(TypeInfo<GIDE::UI::Widgets::Button>::Name).Get<TypeInfo<GIDE::UI::Widgets::Button>::Name>() = value;
        }

        template<>
        void Override<TypeInfo<GIDE::UI::Widgets::Collapsible>::Name>(std::string(&value)())
        {
            Global(TypeInfo<GIDE::UI::Widgets::Collapsible>::Name).Get<TypeInfo<GIDE::UI::Widgets::Collapsible>::Name>() = value;
        }

        template<>
        void Override<TypeInfo<GIDE::UI::Widgets::Label>::Name>(std::string(&value)())
        {
            Global(TypeInfo<GIDE::UI::Widgets::Label>::Name).Get<TypeInfo<GIDE::UI::Widgets::Label>::Name>() = value;
        }
    }

} // namespace GIDE
