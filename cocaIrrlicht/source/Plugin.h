#ifndef PLUGIN_H_INCLUDED
#define PLUGIN_H_INCLUDED

#include <coca/plugin/APlugin.h>

class Plugin : public coca::APlugin
{
public:
    Plugin();
    virtual ~Plugin();

    virtual bool init();
};

#endif // PLUGIN_H_INCLUDED
