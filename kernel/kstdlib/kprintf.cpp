/*
 * Copyright (c) 2020 Jesse Buhagiar <jooster669@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <common/assert.h>
#include <kernel/device/uart/uart.h>
#include <kernel/kstdlib/kstring.h>
#include <libc++/extra.h>
#include <stdarg.h>

static UART uart0(UART::UART0_BASE);
static constexpr const char* digits = "0123456789abcdef";

[[gnu::always_inline]] inline static int kprintf_internal_hex(uint32_t number)
{
    // Stole this from kling, but it's pretty basic
    int ret = 0;
    int shifts = 0;
    for(uint32_t i = number; i > 0; i >>= 4)
        shifts++;

    if(shifts == 0)
        shifts = 1;

    shifts *= 4;
    for(int i = (32 - shifts) / 4; i > 0; i--)
        uart0.putc('0');

    while(shifts > 0)
    {
        shifts -= 4;
        uart0.putc(digits[(number >> shifts) & 0xf]);
    }

    return ret;
}

[[gnu::always_inline]] inline static int kprintf_internal_number(int number)
{
    int ret = 0;
    char buff[512];

    if(number < 0)
    {
        uart0.putc('-');
        number = -number;
    }

    memset(buff, 0, 512);
    int i = 0;
    do
    {
        buff[i++] = '0' + (number % 10);
        number /= 10;
        ret++;
    } while(number);

    int len = strlen(buff);
    UNUSED(len);
    for(int i = strlen(buff) - 1; i >= 0; i--)
        uart0.putc(buff[i]);

    return ret;
}

static int kprintf_internal(const char*& fmt, const va_list& arg)
{
    int num_written = 0;

    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            const char char_ptr = *++fmt;
            switch(char_ptr)
            {
                case 'c':
                {
                    char c = va_arg(arg, int);
                    uart0.putc(c);
                    num_written++;
                    break;
                }
                case 's':
                {
                    const char* str = va_arg(arg, const char*);
                    uart0.puts(str);
                    num_written += strlen(str);
                    break;
                }
                case 'd':
                {
                    int val = va_arg(arg, int);
                    num_written += val < 0 ? kprintf_internal_number(val) + 1 : kprintf_internal_number(val);
                    break;
                }
                case 'x':
                {
                    uint32_t val = va_arg(arg, uint32_t);
                    num_written += kprintf_internal_hex(val);
                    break;
                }
                case '%':
                {
                    uart0.putc('%');
                    num_written++;
                    break;
                }
                default:
                    ASSERT_NOT_REACHED();
                    break;
            }

            fmt++;
        }
        else
        {
            // Is this inefficient???
            uart0.putc(*fmt);
            num_written++;
            fmt++;
        }
    }

    return num_written;
}

int kprintf(const char* fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);
    int ret = kprintf_internal(fmt, arg);
    va_end(arg);

    return ret;
}
