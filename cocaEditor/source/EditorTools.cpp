// Copyright (C) 2007-2008 Harm Boschloo

#include "EditorTools.h"
#include "EditorApplication.h"
#include "EditorFrame.h"
#include "EditorDefs.h"
#include "NodeSelectionDialog.h"
#include "resources.h"
#include <coca/attributes/IAttribute.h>
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include <wx/image.h>

NodeSelectionDialog* EditorTools::_nodeSelectionDialog( 0 );
wxMenu EditorTools::_systemMenu;
wxMenu EditorTools::_nodeMenu;
wxMenu EditorTools::_sinkMenu;
wxMenu EditorTools::_sourceMenu;
wxImageList EditorTools::_icons( 16, 16 );

bool EditorTools::enterSystemDescription( std::string& description )
{
    wxTextEntryDialog dialog( wxGetApp().GetTopWindow(), "the system description", "system description",
                              description.c_str(), ( wxOK | wxCANCEL | wxCENTRE | wxTE_MULTILINE | wxRESIZE_BORDER ) );

    if ( dialog.ShowModal() != wxID_OK ) { return false; }
    description = dialog.GetValue().c_str();

    return true;
}

EEditorId EditorTools::selectNode( std::string& componentId )
{
    NodeSelectionDialog* dialog = getNodeSelectionDialog();
    if ( dialog->ShowModal() != wxID_OK ) { return E_ID_CANCEL; }

    if ( dialog->isComponentNodeSelected() )
    {
        componentId =  dialog->getComponentId().c_str();
        return E_ID_COMPONENT_NODE;
    }
    else if ( dialog->isCompositeNodeSelected() )
    {
        return E_ID_COMPOSITE_NODE;
    }
    else
    {
        return E_ID_CANCEL;
    }
}

bool EditorTools::enterNodeName( std::string& name )
{
    wxTextEntryDialog dialog( wxGetApp().GetTopWindow(), "please specify a name", "node name",
                              name.c_str(), ( wxOK | wxCANCEL | wxCENTRE ) );

    if ( dialog.ShowModal() != wxID_OK ) { return false; }
    name = dialog.GetValue().c_str();

    return true;
}

EEditorId EditorTools::confirmRemove( const coca::INode& node )
{
    wxString text( "Remove node " + getName( node ) + "?" );
    switch ( wxMessageBox( text, "Confirm", wxYES_NO, wxGetApp().GetTopWindow() ) )
    {
    case wxYES:
        return E_ID_YES;
    case wxNO:
        return E_ID_NO;
    default:
        return E_ID_CANCEL;
    }
}

wxString EditorTools::getName( const coca::INode& node )
{
    if ( node.isComponentNode() )
    {
        return ( node.getName() + " [" + node.getComponentId() + "]" );
    }
    else
    {
        return node.getName();
    }
}

wxString EditorTools::getName( const coca::IAttribute& attribute )
{
    COCA_ASSERT( attribute.getOwner() );
    return getName( *attribute.getOwner() ) + ":" + attribute.getOwner()->findAttributeName( &attribute );
}

EImageIndex EditorTools::getImageIndex( const coca::INode& node )
{
    if ( node.isComponentNode() )
    {
        return ( node.isEnabled() ? E_COMPONENT_IMAGE : E_COMPONENT_IMAGE_DISABLED );
    }
    else
    {
        return ( node.isEnabled() ? E_COMPOSITE_IMAGE : E_COMPOSITE_IMAGE_DISABLED );
    }
}

wxColour EditorTools::getTextColour( const coca::INode& node )
{
    //return ( node.isEnabled() ? *wxBLACK : *wxLIGHT_GREY );
    return ( node.isEnabled() ? wxColour( 0, 0, 0 ) : wxColour( 150, 150, 150 ) );
}

// from aui\auibar.cpp
wxBitmap EditorTools::makeDisabledBitmap( wxImage image )
{
    int mr = image.GetMaskRed();
    int mg = image.GetMaskGreen();
    int mb = image.GetMaskBlue();

    unsigned char* data = image.GetData();
    int width = image.GetWidth();
    int height = image.GetHeight();
    bool hasMask = image.HasMask();

    for ( int y = height-1; y >= 0; --y )
    {
        for ( int x = width-1; x >= 0; --x )
        {
            data = image.GetData() + ( y*( width*3 ) )+( x*3 );
            unsigned char* r = data;
            unsigned char* g = data+1;
            unsigned char* b = data+2;

            if ( hasMask && *r == mr && *g == mg && *b == mb )
                continue;

            *r = blendColour( *r, 255, 0.4 );
            *g = blendColour( *g, 255, 0.4 );
            *b = blendColour( *b, 255, 0.4 );
        }
    }

    return wxBitmap( image );
}

void EditorTools::updateFrameTitle( wxString title )
{
    if ( title.IsEmpty() ) { title = wxGetApp().GetAppName(); }
    else { title += " - " + wxGetApp().GetAppName(); }
    wxGetApp().getFrame()->SetTitle( title );
}

wxWindow* EditorTools::getTopWindow()
{
    return wxGetApp().GetTopWindow();
}

NodeSelectionDialog* EditorTools::getNodeSelectionDialog()
{
    if ( !_nodeSelectionDialog )
    {
        _nodeSelectionDialog = new NodeSelectionDialog( wxGetApp().GetTopWindow(),
                E_ID_COMPONENT_SELECTION_DIALOG, "please select a component" );
    }

    return _nodeSelectionDialog;
}


wxMenu& EditorTools::getSystemMenu()
{
    if ( _systemMenu.GetTitle().IsEmpty() )
    {
        _systemMenu.SetTitle( "system" );
        _systemMenu.Append( E_ID_EDIT_SYSTEM_DESCRIPTION, "edit description" );
        _systemMenu.Append( E_ID_EDIT_SYSTEM_ROOT, "set root" );
    }

    return _systemMenu;
}

wxMenu& EditorTools::getNodeMenu()
{
    if ( _nodeMenu.GetTitle().IsEmpty() )
    {
        _nodeMenu.SetTitle( "node" );
        _nodeMenu.Append( E_ID_EDIT_NODE_NAME, "edit name" );
        _nodeMenu.Append( E_ID_ENABLE_NODE, "enable" );
        _nodeMenu.Append( E_ID_DISABLE_NODE, "disable" );
        _nodeMenu.Append( E_ID_MOVE_NODE_UP, "move up" );
        _nodeMenu.Append( E_ID_MOVE_NODE_DOWN, "move down" );
        _nodeMenu.AppendSeparator();
        _nodeMenu.Append( E_ID_APPEND_NODE, "append node" );
        _nodeMenu.AppendSeparator();
        _nodeMenu.Append( wxID_COPY, "copy" );
        _nodeMenu.Append( wxID_PASTE, "paste" );
        _nodeMenu.AppendSeparator();
        _nodeMenu.Append( E_ID_REMOVE_NODE, "remove" );
    }

    return _nodeMenu;
}

wxMenu& EditorTools::getSinkMenu()
{
    if ( _sinkMenu.GetTitle().IsEmpty() )
    {
        _sinkMenu.SetTitle( "sink" );
        _sinkMenu.Append( E_ID_GO_TO_SINK, "go to" );
        _sinkMenu.AppendSeparator();
        _sinkMenu.Append( E_ID_REMOVE_SINK, "remove" );
    }

    return _sinkMenu;
}

wxMenu& EditorTools::getSourceMenu()
{
    if ( _sourceMenu.GetTitle().IsEmpty() )
    {
        _sourceMenu.SetTitle( "source" );
        _sourceMenu.Append( E_ID_GO_TO_SOURCE, "go to" );
        _sourceMenu.AppendSeparator();
        _sourceMenu.Append( E_ID_REMOVE_SOURCE, "remove" );
    }

    return _sourceMenu;
}

wxImageList& EditorTools::getIcons()
{
    if ( _icons.GetImageCount() == 0 )
    {
        // icons
        _icons.Add( wxIcon( coca_xpm ) );
        _icons.Add( wxIcon( composite_xpm ) );
        _icons.Add( wxIcon( component_xpm ) );
        _icons.Add( makeDisabledBitmap( wxImage( coca_xpm ) ) );
        _icons.Add( makeDisabledBitmap( wxImage( composite_xpm ) ) );
        _icons.Add( makeDisabledBitmap( wxImage( component_xpm ) ) );
    }

    return _icons;
}

// private //

unsigned char EditorTools::blendColour( unsigned char fg, unsigned char bg, double alpha )
{
    double result = bg + ( alpha * ( fg - bg ) );
    if ( result < 0.0 )
        result = 0.0;
    if ( result > 255 )
        result = 255;
    return ( unsigned char )result;
}


