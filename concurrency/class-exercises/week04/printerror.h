/* printerror.h */
#ifndef __PRINTERROR_H__
#define __PRINTERROR_H__

/* a causa dell'uso di strerror_r
   dove incluso, compilare con -D_POSIX_C_SOURCE >= 200112L
*/

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif

#ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 200112L
#else
        #if _POSIX_C_SOURCE < 200112L
                #undef _POSIX_C_SOURCE
                #define _POSIX_C_SOURCE 200112L
        #endif
#endif

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 
#include <errno.h>
#include <string.h>	/* per strerror_r  and  memset */

#define myMSGLEN 128

#define PrintERROR_andExit( ERRORCODE, STRMSG )				\
	do {								\
		char errmsg[myMSGLEN];					\
		memset( errmsg, 0, (size_t)myMSGLEN );			\
		strerror_r( ERRORCODE, errmsg, (size_t)myMSGLEN );	\
		printf("%s: errorcode %d %s - Quit\n",			\
			STRMSG, ERRORCODE, errmsg );			\
		exit(1);						\
	} while(0)

#define PrintErrnoAndExit( STRMSG ) 					\
	do {	int myerrno;						\
		char errmsg[myMSGLEN];					\
		myerrno = errno;					\
		memset( errmsg, 0, (size_t)myMSGLEN );			\
		strerror_r( myerrno, errmsg, (size_t)myMSGLEN );	\
		printf("%s: errno %d %s - Quit\n",			\
			STRMSG, myerrno, errmsg );			\
		exit(1);						\
	} while(0)


#endif   /* __PRINTERROR_H__  */


