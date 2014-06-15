// Copyright (C) 2007-2008 Harm Boschloo

#ifndef EDITOR_TOOLS_H_INCLUDED
#define EDITOR_TOOLS_H_INCLUDED

#include "EditorDefs.h"
#include <coca/INode.h>
#include <wx/menu.h>
#include <wx/imaglist.h>

class NodeSelectionDialog;

class EditorTools
{
public:
    static bool enterSystemDescription( std::string& description );
    static EEditorId selectNode( std::string& componentId );
    static bool enterNodeName( std::string& name );
    static EEditorId confirmRemove( const coca::INode& node );

    static wxString getName( const coca::INode& node );
    static wxString getName( const coca::IAttribute& attribute );
    static EImageIndex getImageIndex( const coca::INode& node );
    static wxColour getTextColour( const coca::INode& node );
    static wxBitmap makeDisabledBitmap( wxImage image );
    static void updateFrameTitle( wxString title = "" );

    static wxWindow* getTopWindow();
    static NodeSelectionDialog* getNodeSelectionDialog();
    static wxMenu& getSystemMenu();
    static wxMenu& getNodeMenu();
    static wxMenu& getSinkMenu();
    static wxMenu& getSourceMenu();
    static wxImageList& getIcons();

private:
    static unsigned char blendColour( unsigned char fg, unsigned char bg, double alpha );

    static NodeSelectionDialog* _nodeSelectionDialog;
    static wxMenu _systemMenu;
    static wxMenu _nodeMenu;
    static wxMenu _sinkMenu;
    static wxMenu _sourceMenu;
    static wxImageList _icons;

};

#endif // EDITOR_TOOLS_H_INCLUDED
