/**
 * CPU related classes for the Cortex-A15 Processor
 */
#pragma once

#include <kernel/irqhandler.h>
#include <stdint.h>

#define CAUSE_ILLEGAL_INSTRUCTION __asm__ volatile("bad:\n.word 0xffffffff\n")
#define CAUSE_DATA_ABORT __asm__ volatile("mov r0,#0x00\nldr r0, [r0, #-4]")
#define CAUSE_PREFETCH_ABORT __asm__ volatile("bkpt")

[[noreturn]] static inline void halt()
{
    __asm__ volatile("cpsid iaf"); // Disable all interrupts (IRQ, FIQ and **)
    __asm__ volatile("wfi");       // Wait for an interrupt (which can never happen!)
    for(;;)
        ;
}

// Disable FIQ and IRQs
static inline void cli()
{
    __asm__ volatile("cpsid iaf"); // Disable all interrupts (IRQ, FIQ and **)
}

static inline void sti()
{
    __asm__ volatile("cpsie iaf");
}

struct RegisterDump
{
    uint32_t spsr;
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, fp, r12, sp, lr;
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

class IRQHandler;

void install_handler(uint8_t, IRQHandler&);
void remove_handler(uint8_t);

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

    ProcessorMode get_current_mode() const { return static_cast<ProcessorMode>(m_cpsr & 0x0F); }
    ProcessorMode get_pre_exception_mode() const { return static_cast<ProcessorMode>(m_spsr & 0x0F); }

private:
    uint32_t m_cpsr { 0 };
    uint32_t m_spsr { 0 };
};

extern "C" void relocate_vector_table();
extern "C" void* vector_table;

static constexpr uint32_t ENTRIES_PER_L2_TRANSLATION_TABLE = 256;
static constexpr uint32_t ENTRIES_PER_L1_TRANSLATION_TABLE = 4096;
static constexpr uint32_t PAGE_SIZE = 4096;

union L1TableEntry
{
    struct
    {
        uint32_t always_1 : 1;
        uint32_t always_0 : 1;
        uint32_t pxn : 1;
        uint32_t ns : 1;
        uint32_t sbz : 1;
        uint32_t domain : 4;
        uint32_t unused : 1;
        uint32_t table_base_address : 22;
    };
    uint32_t entry;
};

union L2TableEntry
{
    struct
    {
        uint32_t xn : 1;
        uint32_t always_1 : 1;
        uint32_t b : 1;
        uint32_t c : 1;
        uint32_t ap_lo : 2;
        uint32_t tex : 3;
        uint32_t ap_hi : 1;
        uint32_t s : 1;
        uint32_t nG : 1;
        uint32_t page_base_address : 20;
    };
    uint32_t entry;
};

struct L2TranslationTable
{
    L2TableEntry entries[ENTRIES_PER_L2_TRANSLATION_TABLE];
};

struct L1TranslationTable
{
    L1TableEntry entries[ENTRIES_PER_L1_TRANSLATION_TABLE];
};
