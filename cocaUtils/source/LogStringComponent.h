#ifndef LOG_STRING_COMPONENT_H_INCLUDED
#define LOG_STRING_COMPONENT_H_INCLUDED

#include <coca/IComponent.h>

class LogStringComponent : public coca::IComponent
{
public:
    LogStringComponent();
    ~LogStringComponent();

    bool init( coca::INode& node );
    void onEnabled();
    void onDisabled();

    void log( std::string data );

private:
    std::string _prefix;
    std::string _suffix;
};

#endif // LOG_STRING_COMPONENT_H_INCLUDED
