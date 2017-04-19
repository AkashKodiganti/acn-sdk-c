#include "debug.h"

#include <stdarg.h>
#include <stdio.h>

char dbg_buffer[DBG_LINE_SIZE] __attribute__((weak));

__attribute__((weak)) void dbg_line(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  *dbg_buffer = 0x0;
  vsnprintf(dbg_buffer, DBG_LINE_SIZE-2, fmt, args);
  strcat(dbg_buffer, "\r\n");
  printf(dbg_buffer);
  va_end(args);
}
