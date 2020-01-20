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

struct register_dump
{
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, fp, r12, lr, pc;
    uint32_t spsr;
};

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

extern "C" void relocate_vector_table();
extern "C" void* vector_table;

class CPUID
{
public:
    inline CPUID()
    {
        __asm__ volatile("mrc p15, 0, %[result], c0, c0, 0" : [result] "=r" (m_MID));

        m_implementer =         (m_MID >> 24);
        m_variant =             (m_MID >> 20) & 0xF;
        m_arch =                (m_MID >> 16) & 0xF;
        m_primary_part_number = (m_MID >> 4)  & 0xFFF; 
        m_rev =                  m_MID        & 0xF;
    }

    // TODO: create a decoder for the information extracted

    uint32_t m_MID;                   // 32-bits

    uint8_t  m_implementer;           // 8-bits
    uint8_t  m_variant;               // 4-bits
    uint8_t  m_arch;                  // 4-bits
    uint16_t m_primary_part_number;   // 12-bits
    uint8_t  m_rev;                   // 4-bits

};
