// Copyright (C) 2007-2008 Harm Boschloo

#include "EditorApplication.h"
#include "EditorFrame.h"
#include "EditorDefs.h"
#include "CocaDocument.h"
#include "CocaSystemEditorView.h"
#include "ScopedWXBusyCursor.h"
#include <coca/IFactory.h>
#include <coca/plugin/IPluginManager.h>
#include <coca/utils/staticCast.h>
#include <coca/log.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <set>

/// @todo check http://docs.wxwidgets.org/2.8.6/wx_wxdataobjectoverview.html for copy/paste; drag/drop

// Log //

coca::FileLogMessageHandler _log( "cocaEditor.log" );

// EditorPaths //

wxString EditorApplication::EditorPaths::GetPluginsDir() const
{
    COCA_DEBUG_INFO( "EditorPaths::GetPluginsDir" );
    return wxString( coca::getPluginManager().getDefaultPluginDirectory() );
}

// EditorTraits //

wxStandardPaths& EditorApplication::EditorTraits::GetStandardPaths()
{
    return _paths;
}

// EditorApplication //

IMPLEMENT_APP( EditorApplication );

EditorApplication::EditorApplication() :
        _config( 0 )
{
    wxFileName path( GetTraits()->GetStandardPaths().GetExecutablePath() );
    path.SetExt( "ini" );
    _config = new wxFileConfig( "cocaEditor", "Harm Boschloo", path.GetFullPath(), wxEmptyString,
                                wxCONFIG_USE_LOCAL_FILE );

    COCA_DEBUG_INFO( "Editor Created" );
}

EditorApplication::~EditorApplication()
{
    delete _config;
    COCA_DEBUG_INFO( "Editor Destroyed" );
}

wxAppTraits* EditorApplication::CreateTraits()
{
    COCA_DEBUG_INFO( "EditorApplication::CreateTraits" );
    return new EditorTraits(); // destroyed by wxWidgets
}

bool EditorApplication::OnInit()
{
    COCA_DEBUG_INFO( "EditorApplication::OnInit" );

    SetAppName( "coca editor" );

    // coca resource manager
    _resourceManager.createThread();

    // frame
    EditorFrame* frame = new EditorFrame( 0, E_ID_EDITOR_FRAME, "coca editor",
                                          wxDefaultPosition, wxSize( 800, 600 ) );
    SetTopWindow( frame );

    readConfig();
    loadPlugins();
    createDocumentTemplates();
    _documentManager.SetMaxDocsOpen( 1 );

    frame->Show();

    COCA_DEBUG_INFO( "EditorApplication::OnInit finished" );

    return true;
}

int EditorApplication::OnExit()
{
    COCA_DEBUG_INFO( "EditorApplication::OnExit" );

    writeConfig();
    _resourceManager.destroyThread();

    return 0;
}

void EditorApplication::ExitMainLoop()
{
    COCA_DEBUG_INFO( "EditorApplication::ExitMainLoop()" );
    wxApp::ExitMainLoop();
}

wxConfigBase& EditorApplication::getConfig()
{
    return *_config;
}

CocaResourceManager& EditorApplication::getResourceManager()
{
    return _resourceManager;
}

CocaDocumentManager& EditorApplication::getDocumentManager()
{
    return _documentManager;
}

EditorFrame* EditorApplication::getFrame()
{
    return coca::staticCast<EditorFrame*>( GetTopWindow() );
}

// private //

void EditorApplication::readConfig()
{
    /*wxFileName path( GetTraits()->GetStandardPaths().GetExecutablePath() );
    path.SetExt( "ini" );
    wxFileConfig config( "cocaEditor", "Harm Boschloo", path.GetFullPath(), wxEmptyString,
                         wxCONFIG_USE_LOCAL_FILE );*/

    // file history
    _documentManager.FileHistoryLoad( *_config );
}

void EditorApplication::writeConfig()
{
    /*wxFileName path( GetTraits()->GetStandardPaths().GetExecutablePath() );
    path.SetExt( "ini" );
    wxFileConfig config( "cocaEditor", "Harm Boschloo", path.GetFullPath(), wxEmptyString,
                         wxCONFIG_USE_LOCAL_FILE );*/

    // file history
    _documentManager.FileHistorySave( *_config );
}

void EditorApplication::loadPlugins()
{
    ScopedWXBusyCursor busy;

    wxString dir = GetTraits()->GetStandardPaths().GetPluginsDir();
    int count = coca::getPluginManager().loadPlugins( dir.c_str() );
    COCA_INFO( count << " plugins loaded from " << dir );
}

void EditorApplication::createDocumentTemplates()
{
    coca::IFactory& factory = coca::getFactory();
    std::set<std::string> extensions;

    // readers
    {
        const coca::IFactory::ReaderRegistry::CreatorMap& readerRegistrations =
            factory.getReaderRegistry().getRegistrations();
        coca::IFactory::ReaderRegistry::CreatorMap::const_iterator it;
        for ( it = readerRegistrations.begin(); it != readerRegistrations.end(); ++it )
        {
            extensions.insert( it->first );
        }
    }

    // writers
    {
        const coca::IFactory::WriterRegistry::CreatorMap& writerRegistrations =
            factory.getWriterRegistry().getRegistrations();
        coca::IFactory::WriterRegistry::CreatorMap::const_iterator it;
        for ( it = writerRegistrations.begin(); it != writerRegistrations.end(); ++it )
        {
            extensions.insert( it->first );
        }
    }

    // create templates
    std::set<std::string>::const_iterator it;
    for ( it = extensions.begin(); it != extensions.end(); ++it )
    {
        wxString extension( *it );
        wxString description( "coca " + extension + " document" );
        wxString filter( "*." + extension );
        new wxDocTemplate( &_documentManager, description, filter, "", extension,
                           "coca document", "coca system editor view",
                           CLASSINFO( CocaDocument ), CLASSINFO( CocaSystemEditorView ) );
    }
}
