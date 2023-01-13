


#ifndef _GIDE_INTERNALS_H
#define _GIDE_INTERNALS_H

namespace GIDE
{
    namespace detail
    {
        template<class CallT>
        struct GlobalValue
        {
            CallT Default;

            GlobalValue(CallT& Default_) : Default(Default_) {}

            template<CallT>
            CallT& Get()
            {
                static CallT result = Default;
                return result;
            }
        };

        template<class CallT>
        GlobalValue<CallT> Global(CallT Default)
        {
            return GlobalValue<CallT>(Default);
        }
    }
} // namespace GIDE


#endif // _GIDE_INTERNALS_H

