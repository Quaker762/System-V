/**
 * 
 */
#pragma once

#include "kprintf.cpp"

#ifdef KERNEL
    #define printf kprintf
#endif
