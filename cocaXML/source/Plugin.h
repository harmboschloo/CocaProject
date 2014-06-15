#ifndef XML_PLUGIN_H_INCLUDED
#define XML_PLUGIN_H_INCLUDED

#include <coca/plugin/APlugin.h>

class Plugin : public coca::APlugin
{
public:
    Plugin();
    virtual ~Plugin();

    virtual bool init();
};

#endif // XML_PLUGIN_H_INCLUDED
