// Copyright (C) 2007-2008 Harm Boschloo

#ifndef EDITOR_APPLICATION_H_INCLUDED
#define EDITOR_APPLICATION_H_INCLUDED

#include "CocaResourceManager.h"
#include "CocaDocumentManager.h"
#include "WXLogMessageRouter.h"
#include <wx/app.h>
#include <wx/apptrait.h>
#include <wx/stdpaths.h>

class wxConfigBase;
class EditorFrame;

class EditorApplication : public wxApp
{
public:
    EditorApplication();
    ~EditorApplication();

    wxAppTraits* CreateTraits();
    bool OnInit();
    int OnExit();
    void ExitMainLoop();

    wxConfigBase& getConfig();
    CocaResourceManager& getResourceManager();
    CocaDocumentManager& getDocumentManager();
    EditorFrame* getFrame();

private:
    class EditorPaths : public wxStandardPaths
    {
        virtual wxString GetPluginsDir() const;
    };

    class EditorTraits : public wxGUIAppTraits
    {
    public:
        virtual wxStandardPaths & GetStandardPaths();

    private:
        EditorPaths _paths;
    };

    void readConfig();
    void writeConfig();
    void loadPlugins();
    void createDocumentTemplates();

    wxConfigBase* _config;
    CocaResourceManager _resourceManager;
    WXLogMessageRouter _logMessageRouter;
    CocaDocumentManager _documentManager;
};

DECLARE_APP( EditorApplication );

#endif // EDITOR_APPLICATION_H_INCLUDED
