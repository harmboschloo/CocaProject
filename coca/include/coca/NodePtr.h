// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_NODE_PTR_H_INCLUDED
#define COCA_NODE_PTR_H_INCLUDED

#include "utils/ReferencePointer.h"

namespace coca
{
    class INode;
    typedef ReferencePointer<INode> NodePtr;

} // namespace coca

#endif //COCA_NODE_PTR_H_INCLUDED
