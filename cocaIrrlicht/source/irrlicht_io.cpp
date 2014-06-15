#include "irrlicht_io.h"
#include <coca/log/log.h>
#include <limits>

std::istream& operator>>( std::istream& stream, irr::core::dimension2du& dimension )
{
    return stream >> dimension.Width >> dimension.Height;
}

std::ostream& operator<<( std::ostream& stream, const irr::core::dimension2du& dimension )
{
    return stream << dimension.Width << " " << dimension.Height;
}

std::istream& operator>>( std::istream& stream, irr::video::SColor& color )
{
    irr::u32 value = 0;
    stream >> value;
    color.setAlpha( value );
    stream >> value;
    color.setRed( value );
    stream >> value;
    color.setGreen( value );
    stream >> value;
    color.setBlue( value );
    return stream;
}

std::ostream& operator<<( std::ostream& stream, const irr::video::SColor& color )
{
    return stream << color.getAlpha() << " " << color.getRed() << " " << color.getGreen() << " " << color.getBlue();
}

std::istream& operator>>( std::istream& stream, irr::core::vector3df& vector )
{
    return stream >> vector.X >> vector.Y >> vector.Z;
}

std::ostream& operator<<( std::ostream& stream, const irr::core::vector3df& vector )
{
    stream.precision( std::numeric_limits<irr::f32>::digits10 );
    return stream << vector.X << " " << vector.Y << " " << vector.Z;
}

std::istream& operator>>( std::istream& stream, irr::video::E_DRIVER_TYPE& driverType )
{
    int type = 0;
    stream >> type;
    if ( stream.fail() ) { return stream; }
    switch( type )
    {
        case irr::video::EDT_NULL:
            driverType = irr::video::EDT_NULL;
            break;
        case irr::video::EDT_SOFTWARE:
            driverType = irr::video::EDT_SOFTWARE;
            break;
		case irr::video::EDT_BURNINGSVIDEO:
            driverType = irr::video::EDT_BURNINGSVIDEO;
            break;
		case irr::video::EDT_DIRECT3D8:
            driverType = irr::video::EDT_DIRECT3D8;
            break;
		case irr::video::EDT_DIRECT3D9:
            driverType = irr::video::EDT_DIRECT3D9;
            break;
		case irr::video::EDT_OPENGL:
            driverType = irr::video::EDT_OPENGL;
            break;
        default:
            COCA_ERROR( "Unknown irrlicth driver type: " << type );
            stream.setstate( std::ios::badbit );
            break;
    }
    return stream;
}
