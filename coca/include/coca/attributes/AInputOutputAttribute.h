// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_A_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
#define COCA_A_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED

#include "AOutputAttribute.h"
#include "../utils/staticCast.h"

namespace coca
{
    template<typename ValueType>
    class AInputOutputAttribute : public AOutputAttribute<ValueType>
    {
    public:
        AInputOutputAttribute( bool multipleSources )
        {
            this->initSources( multipleSources );
        }

        virtual ~AInputOutputAttribute()
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

#endif //COCA_A_INPUT_OUTPUT_ATTRIBUTE_H_INCLUDED
