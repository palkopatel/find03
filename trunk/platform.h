#ifndef _PLATFORM_H
#define _PLATFORM_H

/**************v linux eto nado zakommentirovat'******************/
/*#define _MSDOS_*/
/*****************************************************************/

#if defined(_MSDOS_) || defined(__CYGWIN__)
  #ifndef __TURBOC__
    #define __TURBOC__
  #endif
  #include <io.h>
  #include <fcntl.h>
  #define PLATFORM_STRING_END 1
#else
  #include <sys/io.h>
  #include <sys/fcntl.h>
  #define PLATFORM_STRING_END 0
#endif

#endif
