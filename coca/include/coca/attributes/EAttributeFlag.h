// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_E_ATTRIBUTE_FLAG_H_INCLUDED
#define COCA_E_ATTRIBUTE_FLAG_H_INCLUDED

namespace coca
{
    enum EAttributeFlag
    {
        /// Attribute is enabled.
        E_ATTRIBUTE_ENABLED_FLAG                    = (1<<0),
        /// Attribute is enabled.
        E_ATTRIBUTE_PARENT_ENABLED_FLAG              = (1<<1),
        /// Attribute is currently updating.
        E_ATTRIBUTE_IS_UPDATING_FLAG                 = (1<<2),
        /// Input attribute can have multiple sources.
        E_ATTRIBUTE_CAN_HAVE_MULTIPLE_SOURCES_FLAG   = (1<<3),
        /// Attribute automatically calls IAttribute::update() when attribute value is changed.
        E_ATTRIBUTE_AUTO_UPDATE_FLAG                 = (1<<4)
    };

} // namespace coca

#endif //COCA_E_ATTRIBUTE_FLAG_H_INCLUDED
