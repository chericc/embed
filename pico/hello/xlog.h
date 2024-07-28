#ifndef __LOG_H__
#define __LOG_H__

#include <stdint.h>

typedef enum {
    XLOG_LEVEL_TRACE = 1 << 0,
    XLOG_LEVEL_DEBUG = 1 << 1,
    XLOG_LEVEL_LOG = 1 << 2,
    XLOG_LEVEL_INFORMATION = 1 << 3,
    XLOG_LEVEL_WARNING = 1 << 4,
    XLOG_LEVEL_ERROR = 1 << 5,
    XLOG_LEVEL_CRITICAL = 1 << 6,
    XLOG_LEVEL_BUTT = 1 << 7,
} XLOG_LEVEL;

void xlog(XLOG_LEVEL level, const char* file, int line, const char* func,
          const char* format, ...);

#define xlog_trc(...) \
    xlog(XLOG_LEVEL_TRACE, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define xlog_dbg(...) \
    xlog(XLOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define xlog_log(...) \
    xlog(XLOG_LEVEL_LOG, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define xlog_inf(...)                                              \
    xlog(XLOG_LEVEL_INFORMATION, __FILE__, __LINE__, __FUNCTION__, \
         ##__VA_ARGS__)
#define xlog_war(...) \
    xlog(XLOG_LEVEL_WARNING, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define xlog_err(...) \
    xlog(XLOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define xlog_cri(...) \
    xlog(XLOG_LEVEL_CRITICAL, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

void hex_dump_a(const char *desc, uint8_t const*buf, int len, const char *func, int line);
#define hex_dump(desc,buf,len) hex_dump_a(desc,buf,len,__func__,__LINE__)

#endif // __LOG_H__