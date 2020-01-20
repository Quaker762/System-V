/**
 * 
 */
#include "kstring.h"

#include <mjlib/extra.h>
#include <stdint.h>

// TODO: Optimise me with assembly please!
void* memcpy(void* src, void* dst, size_t num)
{
    // r0 = src
    // r1 = dst
    // r2 = num

    // If this is the case, we can do a word copy
    if((reinterpret_cast<uint32_t>(src) & 3) == 0 && (reinterpret_cast<uint32_t>(dst) & 3) == 0 && (num & 3) == 0)
    {
        __asm__ volatile(
        "copy:\n"
        "ldr r3, [r0], #4\n" // Load r3 with src[offset]. src += 4
        "str r3, [r1], #4\n" // Store r3 into src[offset]. dst += 4
        "subs r2, r2, #4\n"  // num -= 4
        "bgt copy\n");
    }
    else
    {
        for(size_t i = 0; i < num; i++)
            *reinterpret_cast<uint8_t*>(dst) = *reinterpret_cast<uint8_t*>(src);
    }

    return dst;
}

void* memset(void* ptr, int value, size_t num)
{
    UNUSED(value);
    UNUSED(num);

    // r0 = ptr
    // r1 = value
    // r2 = num
    __asm__ volatile(
    "move:"
    "strb r1, [r0], #1\n"
    "subs r2, r2, #1\n"
    "bgt move\n");

    return ptr;
}

int memcmp(void* src, void* target, size_t num)
{
    const uint8_t* str_ptr = reinterpret_cast<uint8_t*>(src);
    const uint8_t* target_ptr = reinterpret_cast<uint8_t*>(target);

    for(int i = 0; i < static_cast<int>(num); ++i)
    {
        if(*(str_ptr + i) < *(target_ptr + i))
            return -1;
        if(*(str_ptr + i) > *(target_ptr + i))
            return 1;
    }

    return 0;
}

char* strcpy(char* dst, const char* src)
{
    for(size_t i = 0; src[i] != '\0'; i++)
        dst[i] = src[i];

    return dst;
}

int strcmp(const char* src, const char* target)
{
    while(*src && *target)
    {
        src++;
        target++;
    }

    return src - target;
}

size_t strlen(const char* str)
{
    size_t ret = 0;

    while(*str++ != '\0')
        ret++;

    return ret;
}
