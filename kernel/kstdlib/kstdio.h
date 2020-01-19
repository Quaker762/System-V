/**
 * 
 */
#pragma once

int kprintf(const char* fmt, ...);

#ifdef KERNEL
    #define printf kprintf
#endif
