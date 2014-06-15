#ifndef COCA_A_PLUGIN_H_INCLUDED
#define COCA_A_PLUGIN_H_INCLUDED

#include "IPlugin.h"
#include "PluginInfo.h"
#include "../utils/dll.h"

#define COCA_PLUGIN_CREATE_FUNCTION_NAME createCocaPlugin
#define COCA_EXPORT_PLUGIN( PluginClass ) \
    extern "C" COCA_DLL_EXPORT coca::APlugin* COCA_PLUGIN_CREATE_FUNCTION_NAME() { return new PluginClass(); }

namespace coca
{
    class APlugin : public IPlugin
    {
    public:
        virtual ~APlugin () {}

        const IPluginInfo& getInfo() const { return _info; }

        virtual bool init() = 0;

    protected:
        PluginInfo _info;
    };

} // namespace coca

#endif // COCA_A_PLUGIN_H_INCLUDED
