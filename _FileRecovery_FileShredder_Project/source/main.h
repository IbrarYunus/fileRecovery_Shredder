#ifndef __my_main_h
#define __my_main_h


#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE

#include    <sys/types.h>       /* basic system data types */
#include    <sys/time.h>        /* timeval{} for select() */
#include    <time.h>            /* timespec{} for pselect() */
#include    <errno.h>
#include    <fcntl.h>           /* for nonblocking */
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include    <string.h>
#include    <sys/stat.h>        /* for S_xxx file mode constants */
#include    <sys/uio.h>         /* for iovec{} and readv/writev */
#include    <unistd.h>
#include    <sys/un.h>          /* for Unix domain sockets */

#include    <pthread.h>         /* threading */

#include "scan.h"

extern int devfd;

extern struct ext2_filsys *g_fs;

#endif

int totalDirectories;

////////////////////////////////Structs for Directory info//////////

////////////

////////////////////////////////////////////////////////////////////
