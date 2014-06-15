#ifndef COCA_UTILS_FACTORY_REGISTRY_H_INCLUDED
#define COCA_UTILS_FACTORY_REGISTRY_H_INCLUDED

#include "NonCopyable.h"
#include <string>
#include <map>

namespace coca
{
    template<typename BaseType>
    class FactoryRegistry : private NonCopyable
    {
    public:
        typedef BaseType*( *Creator )();
        typedef std::map<const std::string,Creator> CreatorMap;

        FactoryRegistry()
        {
        }

        ~FactoryRegistry()
        {
        }

        bool add( const std::string& id, Creator creator )
        {
            if ( hasRegistration( id ) ) { return false; }
            _registrations[id] = creator;
            return true;
        }

        bool hasRegistration( const std::string& id ) const
        {
            typename CreatorMap::const_iterator it = _registrations.find( id );
            return ( it != _registrations.end() );
        }

        const CreatorMap& getRegistrations() const
        {
            return _registrations;
        }

        void clearRegistrations()
        {
            _registrations.clear();
        }

        BaseType* create( const std::string& id ) const
        {
            typename CreatorMap::const_iterator it = _registrations.find( id );
            if ( it == _registrations.end() ) { return 0; }
            return ( *it->second )();
        }

    private:
        CreatorMap _registrations;
    };

} // namespace coca

#endif // COCA_UTILS_FACTORY_REGISTRY_H_INCLUDED
