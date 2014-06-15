// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaLoggerSentry.h"

// SaveCommand //

CocaLoggerSentry::CocaLoggerSentry()
{
}

CocaLoggerSentry::~CocaLoggerSentry()
{
}

void CocaLoggerSentry::enter()
{
    _cs.Enter();
}

void CocaLoggerSentry::leave()
{
    _cs.Leave();
}
