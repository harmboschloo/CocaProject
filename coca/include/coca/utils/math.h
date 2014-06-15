// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_MATH_H_INCLUDED
#define COCA_MATH_H_INCLUDED

namespace coca
{
    const double PI = 3.14159265358979323846;
    const double DEG2RAD = PI/180;
    const double RAD2DEG = 180/PI;

    template<typename T>
    inline T square( T value )
    {
        return ( value*value );
    }

    template<typename T>
    inline T cubic( T value )
    {
        return ( value*value*value );
    }

    template<typename T>
    inline T sign( T value )
    {
        if ( value < 0 ) { return -1; }
        else if ( value > 0 ) { return 1; }
        else { return 0; }
    }

    template<typename T>
    inline T limit( T value, T min, T max )
    {
        if ( value >= max ) { return max; }
        else if ( value <= min ) { return min; }
        return value;
    }

    template<typename T>
    inline T max( T value1, T value2 )
    {
        return ( value1 > value2 ? value1 : value2 );
    }

    template<typename T>
    inline T min( T value1, T value2 )
    {
        return ( value1 < value2 ? value1 : value2 );
    }

    template<typename T>
    inline T abs( T value )
    {
        return ( value < 0 ? -value : value );
    }

    // angles
    template<typename T>
    inline T limitAngleDegrees( T angle, T minAngle = 0 )
    {
        if ( angle < minAngle ) { return static_cast<T>(angle+360*(1+static_cast<int>((minAngle-angle)/360))); }
        else if ( angle >= ( minAngle+360 ) ) { return static_cast<T>(angle-360*(static_cast<int>((angle-minAngle)/360))); }
        return angle;
    }

    template<typename T>
    inline T limitAngleRadians( T angle, T minAngle = 0 )
    {
        if ( angle < minAngle ) { return static_cast<T>(angle+(2*PI)*(1+static_cast<int>((minAngle-angle)/(2*PI)))); }
        else if ( angle >= ( minAngle+(2*PI) ) ) { return static_cast<T>(angle-(2*PI)*(static_cast<int>((angle-minAngle)/(2*PI)))); }
        return angle;
    }

} // namespace coca

#endif // COCA_MATH_H_INCLUDED
