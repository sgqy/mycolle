
#ifndef __MAKELOG_H__
#define __MAKELOG_H__

// Use eigher of the 2 options to change how to log:
//   Print to stderr:
//     #define ENABLE_LOG
//   Print to file(linux):
//     #define ENABLE_FILE_LOG

#include <stdio.h>
#ifdef ENABLE_FILE_LOG
    #include <string.h>
    #include <unistd.h>
    #include <linux/limits.h>
#endif

#define __LOG_RAW_S2(str) #str
#define __LOG_RAW_S(str) __LOG_RAW_S2(str)
#define __LOG_POS __LOG_RAW_S(__FILE__) "[" __LOG_RAW_S(__LINE__) "]"

#define LOGFILE_IMPL FILE* __logfp
#define LOGFILE_DECL extern LOGFILE_IMPL


#ifdef ENABLE_FILE_LOG
#define LOGFILE_INIT() \
    do {                                               \
        char __logfn[PATH_MAX];                        \
        readlink("/proc/self/exe", __logfn, PATH_MAX); \
        strcat(__logfn, ".log");                       \
        __logfp = fopen(__logfn, "w+");                \
        if(__logfp == 0)                               \
        {                                              \
            __logfp = stderr;                          \
        }                                              \
    } while(0)
#else
#define LOGFILE_INIT() do { __logfp = stderr; } while(0)
#endif

#if (defined ENABLE_LOG) || (defined ENABLE_FILE_LOG)
#define make_log(str...) \
    do {                                                        \
        fprintf(__logfp, "%s <%s>: ", __func__, __LOG_POS); \
        fprintf(__logfp, str);                              \
        fflush(__logfp);                                    \
    } while(0)
#else
#define make_log(str...)
#endif

#endif // __MAKELOG_H__
