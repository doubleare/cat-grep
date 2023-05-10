#ifndef SRC_COMMON_S21_COMMON_H_
#define SRC_COMMON_S21_COMMON_H_
#ifndef _STDIO_H_
#include <stdio.h>
#endif
#ifndef _STRING_H_
#include <string.h>
#endif
#ifndef _ERRNO_H_
#include <errno.h>
#endif
#ifndef _GETOPT_H_
#include <getopt.h>
#endif
#if defined(__linux__)
#define OSIS 1
#elif defined(__APPLE__)
#define OSIS 0
#endif

#endif  // SRC_COMMON_S21_COMMON_H_
