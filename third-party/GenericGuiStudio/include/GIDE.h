#ifndef _GIDE_H
#define _GIDE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// std includes
#include <cstddef>
#include <string>


namespace GIDE
{
    namespace Traits
    {
        enum Types
        {
            SizeUnit,
            PositionUnit,
            ImageData
        };
    }

    template<class, Traits::Types>
    struct Trait;

    template<class ComponentT>
    struct Trait<ComponentT, Traits::SizeUnit> { typedef unsigned long Type; };
    template<class ComponentT>
    struct Trait<ComponentT, Traits::PositionUnit> { typedef unsigned long Type; };
    template<class ComponentT>
    struct Trait<ComponentT, Traits::ImageData> { typedef void* Type; };

    template<class SizeUnitT>
    struct Size {
        typedef SizeUnitT Unit;

        Unit width;
        Unit height;
    };

    template<
        class SizeUnitT
    >
    struct Image {
        typedef Size<SizeUnitT> Size;

        enum Format {

        } format;

        Size size;
    };

    template<class PosUnitT>
    struct Position2D {
        typedef PosUnitT Unit;

        Unit x;
        Unit y;
    };

    namespace System
    {
        struct Display
        {
            typedef Size<Trait<Display, Traits::SizeUnit>::Type> SizeType;

            static SizeType Size(); // returns global display size for GIDE; must be overridden (use GIDE::Override to set implementation)
        };
    }

    namespace RTTI
    {
        template<class T>
        struct TypeInfo
        {
            static std::string Name() { return "widget"; }
        };
    }




    namespace System
    {
        struct Log {

            enum Severity {
                Trace,
                Debug,
                Info,
                Warning,
                Error,
                Fatal
            };

        };

        void Abort(const std::string& message = "", int err = 0, Log::Severity severity = Log::Fatal); // called when fatal error that cannot be handled by GIDE occured (use GIDE::Override to use custom implementation)
        void Print(const std::string& message);      // called when GIDE wants to print message (use GIDE::Override to use custom implementation)

        bool ImplementationReady(); // called when GIDE checks if implemetation is ready, f.e. graphical system is initialized (use GIDE::Override to use custom implementation)

    } // namespace System

    namespace RTTI
    {
        // Override global calls (similar to C++ virtual functions), f.e.: 
        // GIDE::RTTI::Override<GIDE::Print>(my_logging_funtion);
        template<void(&)(const std::string&)>
        void Override(void(&)(const std::string&));


        // GIDE::RTTI::Override<GIDE::Abort>(my_throw_function);
        template<void(&)(const std::string&, int, System::Log::Severity)>
        void Override(void(&)(const std::string&, int, System::Log::Severity));

        template<bool(&)()>
        void Override(bool(&)());

        template<System::Display::SizeType(&)()>
        void Override(System::Display::SizeType(&)());

        template<std::string(&)()>
        void Override(std::string(&)());

    } // namespace RTTI


} // namespace GIDE


#endif // _GIDE_H
