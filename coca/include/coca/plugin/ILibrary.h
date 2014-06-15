// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_I_LIBRARY_H_INCLUDED
#define COCA_I_LIBRARY_H_INCLUDED

#include "../utils/NonCopyable.h"
#include <string>

namespace coca
{
    class ILibrary : private NonCopyable
    {
    public:
        typedef void(*FunctionSymbol)();

        virtual ~ILibrary() {}

        virtual bool load( const std::string& filename ) = 0;
        virtual void unload() = 0;
        virtual bool isLoaded() const = 0;

        virtual const std::string& getFilename() const = 0;
        virtual void* findSymbol( const std::string& symbolName ) const = 0;
    };

} // namespace coca

#endif // COCA_I_LIBRARY_H_INCLUDED
