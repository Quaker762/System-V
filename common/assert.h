/**
 * 
 */
#pragma once

#ifdef KERNEL
    #include <kernel/assertions.h>
#else
    #error "We only currently support Kernel assertions!"
#endif