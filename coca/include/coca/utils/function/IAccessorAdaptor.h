// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_ACCESSOR_ADAPTOR_H_INCLUDED
#define COCA_I_ACCESSOR_ADAPTOR_H_INCLUDED

#include "../ReferenceCounted.h"

namespace coca
{
    template<typename GetReturn, typename SetReturn = void, typename SetArgument = GetReturn>
    class IAccessorAdaptor : public ReferenceCounted
    {
    public:
        virtual ~IAccessorAdaptor() {};

        virtual GetReturn operator()() const = 0;
        virtual SetReturn operator()( SetArgument ) const = 0;
    };

} // namespace coca

#endif // COCA_I_ACCESSOR_ADAPTOR_H_INCLUDED

