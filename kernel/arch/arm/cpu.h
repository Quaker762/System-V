/**
 * CPU related classes for the Cortex-A15 Processor
 */
#pragma once

#include <stdint.h>

[[noreturn]] static inline void halt()
{
    __asm__ volatile("cpsid iaf"); // Disable all interrupts (IRQ, FIQ and **)
    __asm__ volatile("wfi");       // Wait for an interrupt (which can never happen!)
    for(;;)
        ;
}

struct register_dump
{
    uint32_t spsr;
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, fp, r12, lr;
} __attribute__((packed));

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

enum class ProcessorMode : uint8_t
{
    USER = 0b0000,
    FIQ = 0b0001,
    IRQ = 0b0010,
    KERNEL_MODE = 0b0011,
    MONITOR = 0b0110,
    ABORT = 0b0111,
    HYPERVISOR = 0b1010,
    UNDEFINED = 0b1011,
    SYSTEM = 0b1111
};

class StatusRegister
{
public:
    StatusRegister()
    {
        __asm__ volatile("mrs %[result], cpsr"
                         : [result] "=r"(m_cpsr));
        __asm__ volatile("mrs %[result], spsr"
                         : [result] "=r"(m_spsr));
    }

    uint32_t cpsr() const { return m_cpsr; }
    uint32_t spsr() const { return m_spsr; }

    ProcessorMode get_current_mode() const { return static_cast<ProcessorMode>(m_cpsr & 0x03); }
    ProcessorMode get_pre_exception_mode() const { return static_cast<ProcessorMode>(m_spsr & 0x03); }

private:
    uint32_t m_cpsr { 0 };
    uint32_t m_spsr { 0 };
};

extern "C" void
relocate_vector_table();
extern "C" void* vector_table;

class CPUID
{
public:
    inline CPUID()
    {
        __asm__ volatile("mrc p15, 0, %[result], c0, c0, 0"
                         : [result] "=r"(m_MID));

        m_implementer = (m_MID >> 24);
        m_variant = (m_MID >> 20) & 0xF;
        m_arch = (m_MID >> 16) & 0xF;
        m_primary_part_number = (m_MID >> 4) & 0xFFF;
        m_rev = m_MID & 0xF;
    }

    // TODO: create a decoder for the information extracted

    uint32_t m_MID; // 32-bits

    uint8_t m_implementer;          // 8-bits
    uint8_t m_variant;              // 4-bits
    uint8_t m_arch;                 // 4-bits
    uint16_t m_primary_part_number; // 12-bits
    uint8_t m_rev;                  // 4-bits
};
