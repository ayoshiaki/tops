#ifndef CROSS_PLATFORM_HPP
#define CROSS_PLATFORM_HPP

#ifdef WIN32
 #define DLLEXPORT __declspec( dllexport )
#else
 #define DLLEXPORT
#endif

#endif
