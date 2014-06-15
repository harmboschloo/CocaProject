#ifndef BULLET_IO_H_INCLUDED
#define BULLET_IO_H_INCLUDED

#include <btBulletDynamicsCommon.h>
#include <istream>
#include <ostream>

std::istream& operator>>( std::istream& stream, btVector3& vector );
std::ostream& operator<<( std::ostream& stream, const btVector3& vector );

std::istream& operator>>( std::istream& stream, btQuaternion& quaternion );
std::ostream& operator<<( std::ostream& stream, const btQuaternion& quaternion );

#endif // BULLET_IO_H_INCLUDED
