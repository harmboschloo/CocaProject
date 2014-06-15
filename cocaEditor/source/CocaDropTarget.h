// Copyright (C) 2007-2008 Harm Boschloo

#ifndef COCA_DROP_TARGET_H_INCLUDED
#define COCA_DROP_TARGET_H_INCLUDED

#include <coca/utils/function/autogen/Function3.h>
#include <wx/dnd.h>

class CocaDataObject;

class CocaDropTarget : public wxDropTarget
{
public:
    typedef coca::Function<wxDragResult,wxPoint,wxDragResult,const CocaDataObject*> Callback;

    CocaDropTarget( const Callback& callback );
    ~CocaDropTarget();

    // called when the mouse enters the window (only once until OnLeave())
    //virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def)
    //    { return OnDragOver(x, y, def); }

    // called when the mouse moves in the window - shouldn't take long to
    // execute or otherwise mouse movement would be too slow
    wxDragResult OnDragOver( wxCoord x, wxCoord y, wxDragResult def );

    // called when mouse leaves the window: might be used to remove the
    // feedback which was given in OnEnter()
    //virtual void OnLeave() { }

    // called after OnDrop() returns TRUE: you will usually just call
    // GetData() from here and, probably, also refresh something to update the
    // new data and, finally, return the code indicating how did the operation
    // complete (returning default value in case of success and wxDragError on
    // failure is usually ok)
    wxDragResult OnData( wxCoord x, wxCoord y, wxDragResult def );

private:
    Callback _callback;
};

#endif // COCA_DROP_TARGET_H_INCLUDED

