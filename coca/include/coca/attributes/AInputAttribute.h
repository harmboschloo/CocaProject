// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_INPUT_ATTRIBUTE_H_INCLUDED
#define COCA_A_INPUT_ATTRIBUTE_H_INCLUDED

#include "ATypeAttribute.h"
#include "AOutputAttribute.h"
#include "../utils/staticCast.h"

namespace coca
{
    template<typename ValueType>
    class AInputAttribute : public ATypeAttribute<ValueType>
    {
    public:
        AInputAttribute( bool multipleSources )
        {
            this->initSources( multipleSources );
        }

        virtual ~AInputAttribute()
        {
        }

    protected:
        typedef AOutputAttribute<ValueType> SourceAttibuteType;

        SourceAttibuteType& getSourceAttribute( IAttribute& output )
        {
            return *staticCast<SourceAttibuteType*>( &output );
        }
    };

} // namespace coca

#endif //COCA_A_INPUT_ATTRIBUTE_H_INCLUDED
