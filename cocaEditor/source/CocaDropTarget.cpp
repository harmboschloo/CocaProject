// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaDropTarget.h"
#include "CocaDataObject.h"
#include <coca/log/log.h>
#include <coca/utils/staticCast.h>

CocaDropTarget::CocaDropTarget( const Callback& callback ) :
    _callback( callback )
{
    SetDataObject( new CocaDataObject() );
}

CocaDropTarget::~CocaDropTarget()
{
}

wxDragResult CocaDropTarget::OnDragOver( wxCoord x, wxCoord y, wxDragResult def )
{
    return _callback( wxPoint( x, y ), def, 0 );
}

wxDragResult CocaDropTarget::OnData( wxCoord x, wxCoord y, wxDragResult def )
{
    COCA_DEBUG_INFO( "OnData: " << x << "," << y << "; " << def );

    if ( !GetData() ) { return wxDragNone; }

    CocaDataObject* data = coca::staticCast<CocaDataObject*>( m_dataObject );
    if ( !data ) { return wxDragError; }

    return _callback( wxPoint( x, y ), def, data );
}
