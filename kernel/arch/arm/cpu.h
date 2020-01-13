/**
 * CPU related classes for the Cortex-A15 Processor
 */
#pragma once

#include <stdint.h>

[[noreturn]] static inline void halt()
{
    __asm__ volatile("cpsid iaf");  // Disable all interrupts (IRQ, FIQ and **)
    __asm__ volatile("wfi");        // Wait for an interrupt (which can never happen!)
    for(;;);
}

// A clever trick from Serenity OS. 
// This class is instantiated at the beginning of a function
// that should have interrupts disabled. The constructor emits the
// instruction "cpsid iaf", and the destructor emits "cpsie iaf", meaning
// that when the function leaves scope (and hence the object is destroyed)
// interrupts are automagically re-enabled without us having to
// remember to do it ourselves!
class InterruptDisabler final
{
public:
    inline InterruptDisabler()
    {
        __asm__ volatile("cpsid iaf");
    }

    inline ~InterruptDisabler()
    {
        __asm__ volatile("cpsie iaf");
    }
};
