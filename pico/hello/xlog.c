#include "xlog.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

static unsigned int s_log_mask = 0xffffffff;

static const char* const_basename(const char* filepath) {
    const char* base = strrchr(filepath, '/');
    if (!base) base = strrchr(filepath, '\\');
    return base ? (base + 1) : filepath;
}

static const char* xlog_getlevel(XLOG_LEVEL level) {
    int i = 0;

    /* Make sure this two arrays has the same number of items. */

    const char* array_name[] = {"trc", "dbg", "log", "inf",
                                "war", "err", "cri"};
    const XLOG_LEVEL array_level[] = {
        XLOG_LEVEL_TRACE,       XLOG_LEVEL_DEBUG,   XLOG_LEVEL_LOG,
        XLOG_LEVEL_INFORMATION, XLOG_LEVEL_WARNING, XLOG_LEVEL_ERROR,
        XLOG_LEVEL_CRITICAL,
    };

    for (i = 0; i < XLOG_LEVEL_BUTT; ++i) {
        if (array_level[i] == level) {
            return array_name[i];
        }
    }

    return "unknown";
}

void xlog(XLOG_LEVEL level, const char* file, int line, const char* func,
          const char* format, ...) {
    va_list ap;

    if (!(level & s_log_mask)) {
        return;
    }

    time_t now = time(NULL);
    struct tm stm;
    localtime_r(&now, &stm);
    char now_str[64];
    strftime(now_str, sizeof(now_str), "%F %T", &stm);

    do {
        FILE *fp = stdout;
        if (NULL == fp) {
            break;
        }

        fprintf(fp, "[%s]", now_str);
        fprintf(fp, "[%s]", xlog_getlevel(level));
        fprintf(fp, "[%s %d %s] ", const_basename(file), line, func);
        va_start(ap, format);
        vfprintf(fp, format, ap);
        va_end(ap);
        fprintf(fp, "\n");
        fflush(fp);
    } while (0);

    return;
}