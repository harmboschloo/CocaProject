// Copyright (C) 2007-2008 Harm Boschloo

#include "CocaDataObject.h"
#include <wx/utils.h>
#include <cstring>

CocaDataObject::CocaDataObject( const coca::INode* node, const coca::IAttribute* attribute  )
{
    wxString formatId( "cocaData" );
    formatId << ::wxGetProcessId(); // no interproccess dnd for now
    SetFormat( formatId.c_str() );

    //memset( &_data, 0, sizeof( SData ) );
    _data.node = node;
    _data.attribute = attribute;
}

CocaDataObject::~CocaDataObject()
{
}

size_t CocaDataObject::GetDataSize() const
{
    return sizeof( SData );
}

bool CocaDataObject::GetDataHere( void* buffer ) const
{
    memcpy( buffer, &_data, sizeof( SData ) );
    return true;
}

bool CocaDataObject::SetData( size_t size, const void* buffer )
{
    COCA_ASSERT_EQUAL( size, sizeof( SData ) );
    memcpy( &_data, buffer, size );
    return true;
}

const coca::INode* CocaDataObject::getNode() const
{
    return _data.node;
}

const coca::IAttribute* CocaDataObject::getAttribute() const
{
    return _data.attribute;
}
