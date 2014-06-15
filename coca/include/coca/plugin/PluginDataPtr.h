// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PLUGIN_DATA_PTR_H_INCLUDED
#define COCA_PLUGIN_DATA_PTR_H_INCLUDED

#include "../utils/ReferencePointer.h"

namespace coca
{
    class IPluginData;
    typedef ReferencePointer<IPluginData> PluginDataPtr;

} // namespace coca

#endif //COCA_PLUGIN_DATA_PTR_H_INCLUDED
