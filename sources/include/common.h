#ifndef __LITE_DRIVE_COMMON_H__
#define __LITE_DRIVE_COMMON_H__

#if defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC
#elif defined(__linux__)
#define OS_LINUX
#else
#error "Unsupported operating system"
#endif

#ifdef OS_WIN
#define likely(x)       (x)
#define unlikely(x)     (x)
#else
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

#endif // __LITE_DRIVE_COMMON_H__