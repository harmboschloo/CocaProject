#ifndef IRRLICHT_IO_H_INCLUDED
#define IRRLICHT_IO_H_INCLUDED

#include <irrlicht.h>
#include <istream>
#include <ostream>

std::istream& operator>>( std::istream& stream, irr::core::dimension2du& dimension );
std::ostream& operator<<( std::ostream& stream, const irr::core::dimension2du& dimension );

std::istream& operator>>( std::istream& stream, irr::video::SColor& color );
std::ostream& operator<<( std::ostream& stream, const irr::video::SColor& color );

std::istream& operator>>( std::istream& stream, irr::core::vector3df& vector );
std::ostream& operator<<( std::ostream& stream, const irr::core::vector3df& vector );

std::istream& operator>>( std::istream& stream, irr::video::E_DRIVER_TYPE& driverType );

#endif // IRRLICHT_IO_H_INCLUDED
