#include "bullet_io.h"
#include <limits>

std::istream& operator>>( std::istream& stream, btVector3& vector )
{
    return stream >> vector.m_floats[0] >> vector.m_floats[1] >> vector.m_floats[2];
}

std::ostream& operator<<( std::ostream& stream, const btVector3& vector )
{
    stream.precision( std::numeric_limits<btScalar>::digits10 );
    return stream << vector.m_floats[0] << " " << vector.m_floats[1] << " " << vector.m_floats[2];
}

std::istream& operator>>( std::istream& stream, btQuaternion& quaternion )
{
    btScalar scalar( 0 );

    stream >> scalar;
    quaternion.setX( scalar );
    stream >> scalar;
    quaternion.setY( scalar );
    stream >> scalar;
    quaternion.setZ( scalar );
    stream >> scalar;
    quaternion.setW( scalar );

    return stream;
}

std::ostream& operator<<( std::ostream& stream, const btQuaternion& quaternion )
{
    stream.precision( std::numeric_limits<btScalar>::digits10 );
    return stream << quaternion.getX() << " " << quaternion.getY() << " " << quaternion.getZ()
           << " " << quaternion.getW();
}
