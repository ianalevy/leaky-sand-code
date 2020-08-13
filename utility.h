// utility.h

#ifndef UTILTYH
#define UTILTYH
#include <cmath>
#include <cstdlib>
#include <iostream>

void error( const char *s );
void warning( const char *s, std::ostream& out=std::cerr );
inline double sq( double t )
{return t*t;}
inline double abs( double x )
{return x>=0.0 ? x : -x;}

#endif
