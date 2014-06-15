// Copyright (C) 2008-2009 Harm Boschloo

#ifndef COCA_PREPROCESSOR_H_INCLUDED
#define COCA_PREPROCESSOR_H_INCLUDED

#define COCA_DATE_TIME __DATE__ " " __TIME__

#define COCA_HIDE( x )

// From boost (BOOST_JOIN):
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in COCA_DO_DO_JOIN but does in COCA_DO_JOIN.
#define COCA_JOIN( a, b ) COCA_DO_JOIN( a, b )
#define COCA_DO_JOIN( a, b ) COCA_DO_DO_JOIN( a, b )
#define COCA_DO_DO_JOIN( a, b ) a##b

#define COCA_TO_STRING( macro ) COCA_DO_TO_STRING( macro )
#define COCA_DO_TO_STRING( macro ) COCA_DO_DO_TO_STRING( macro )
#define COCA_DO_DO_TO_STRING( macro ) #macro

#define COCA_REPEAT_0( base )
#define COCA_REPEAT_1( base ) base##1
#define COCA_REPEAT_2( base ) COCA_REPEAT_1( base ) , base##2
#define COCA_REPEAT_3( base ) COCA_REPEAT_2( base ) , base##3
#define COCA_REPEAT_4( base ) COCA_REPEAT_3( base ) , base##4
#define COCA_REPEAT_5( base ) COCA_REPEAT_4( base ) , base##5

#define COCA_REPEAT( count, base ) COCA_JOIN( COCA_REPEAT_, count) ( base )

#define COCA_REPEAT_SUFFIX_0( base, suffix )
#define COCA_REPEAT_SUFFIX_1( base, suffix ) COCA_JOIN( base##1, suffix )
#define COCA_REPEAT_SUFFIX_2( base, suffix ) COCA_REPEAT_SUFFIX_1( base, suffix ) , COCA_JOIN( base##2, suffix )
#define COCA_REPEAT_SUFFIX_3( base, suffix ) COCA_REPEAT_SUFFIX_2( base, suffix ) , COCA_JOIN( base##3, suffix )
#define COCA_REPEAT_SUFFIX_4( base, suffix ) COCA_REPEAT_SUFFIX_3( base, suffix ) , COCA_JOIN( base##4, suffix )
#define COCA_REPEAT_SUFFIX_5( base, suffix ) COCA_REPEAT_SUFFIX_4( base, suffix ) , COCA_JOIN( base##5, suffix )

#define COCA_REPEAT_SUFFIX( count, base, suffix ) COCA_JOIN( COCA_REPEAT_SUFFIX_, count) ( base, suffix )

#define COCA_REPEAT2_0( base1, base2 )
#define COCA_REPEAT2_1( base1, base2 ) base1##1 base2##1
#define COCA_REPEAT2_2( base1, base2 ) COCA_REPEAT2_1( base1, base2 ) , base1##2 base2##2
#define COCA_REPEAT2_3( base1, base2 ) COCA_REPEAT2_2( base1, base2 ) , base1##3 base2##3
#define COCA_REPEAT2_4( base1, base2 ) COCA_REPEAT2_3( base1, base2 ) , base1##4 base2##4
#define COCA_REPEAT2_5( base1, base2 ) COCA_REPEAT2_4( base1, base2 ) , base1##5 base2##5

#define COCA_REPEAT2( count, base1, base2 ) COCA_JOIN( COCA_REPEAT2_, count) ( base1, base2 )


#endif // COCA_PREPROCESSOR_H_INCLUDED
