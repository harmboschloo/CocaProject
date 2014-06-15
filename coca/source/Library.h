// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_LIBRARY_H_INCLUDED
#define COCA_LIBRARY_H_INCLUDED

#include <coca/plugin/ILibrary.h>

namespace coca
{
    struct LibraryData;

    class Library : public ILibrary
    {
    public:
        Library();
        ~Library();

        bool load( const std::string& filename );
        void unload();
        bool isLoaded() const;

        const std::string& getFilename() const;
        void* findSymbol( const std::string& symbolName ) const;
    private:
        LibraryData* _data;
    };

} // namespace coca

#endif // COCA_LIBRARY_H_INCLUDED
