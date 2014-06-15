// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_DATA_OBJECT_H_INCLUDED
#define COCA_DATA_OBJECT_H_INCLUDED

#include <coca/INode.h>
#include <wx/dataobj.h>

class CocaDataObject : public wxDataObjectSimple
{
public:
    CocaDataObject( const coca::INode* node = 0, const coca::IAttribute* attribute = 0 );
    ~CocaDataObject();

    size_t GetDataSize() const;
    bool GetDataHere( void* buffer ) const;
    bool SetData( size_t size, const void* buffer );

    const coca::INode* getNode() const;
    const coca::IAttribute* getAttribute() const;

private:
    struct SData
    {
        const coca::INode* node;
        const coca::IAttribute* attribute;
    } _data;

    DECLARE_NO_COPY_CLASS( CocaDataObject )
};

#endif // COCA_DATA_OBJECT_H_INCLUDED

