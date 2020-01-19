/**
 *  Kernel printf implementation
 */
#include <kernel/device/uart.h>
#include <stdarg.h>

static int kprintf_internal(const char*& fmt, const va_list& arg)
{
    const char* char_pointer;
    int num = 0;
}

int kprintf(const char* fmt, ...)
{
    va_list arg;
    
    va_start(arg, fmt);
    int ret = kprintf_internal(fmt, arg);
    va_end(arg);

    return ret;
}