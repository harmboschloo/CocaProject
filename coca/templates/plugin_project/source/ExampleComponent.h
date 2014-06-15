#ifndef EXAMPLE_COMPONENT_H_INCLUDED
#define EXAMPLE_COMPONENT_H_INCLUDED

#include <coca/coca.h>

class ExampleComponent : public coca::IComponent
{
public:
    ExampleComponent();
    virtual ~ExampleComponent();

    virtual bool init( coca::INode& node );
    virtual void onEnabled();
    virtual void onDisabled();

    void setInput( float value );
    double getOutput() const;

private:
    bool _setting;
    double _output;
};

#endif // EXAMPLE_COMPONENT_H_INCLUDED
