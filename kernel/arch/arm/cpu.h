/**
 * CPU related classes for the Cortex-A15 Processor
 */
#pragma once

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

    ProcessorMode get_current_mode() const { return static_cast<ProcessorMode>(m_cpsr & 0x0F); }
    ProcessorMode get_pre_exception_mode() const { return static_cast<ProcessorMode>(m_spsr & 0x0F); }

private:
    uint32_t m_cpsr { 0 };
    uint32_t m_spsr { 0 };
};

extern "C" void relocate_vector_table();
extern "C" void* vector_table;

enum class SCTLRFlag : uint32_t
{
    MMU_ENABLE = 0x1,
    ALIGNMENT_CHECK_ENABLE = 0x2,
    CACHE_ENABLE = 0x4,
    BRANCH_PREDICTION_ENABLE = 0x800,
    INSTRUCTION_CACHE_ENABLE = 0x1000,
    EXCEPTION_VECTOR_TABLE_BASE = 0x2000,
    FIQ_CONFIGURATION_ENABLE = 0x200000,
    ALIGNMENT_MODEL = 0x400000,
    EXCEPTION_ENDIANNESS = 0x2000000,
    NMFI_SUPPORT = 0x8000000,
    THUMB_EXCEPTION_ENABLE = 0x40000000
};

void set_SCTLR_flag(SCTLRFlag flag);
void unset_SCTLR_flag(SCTLRFlag flag);
uint32_t get_SCTLR();

// Helpful info about the DACR -> http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0211i/I1039311.html
// According to the programmers guide, the use of domains is deprecated in ARMv7 and we is recommended to set
// all domain ID fields to 0 and set all the fields in the DACR to 'Client'
enum class DACRValue : uint32_t
{
    NO_ACCESS = 0x00000000,
    CLIENT = 0x55555555,
    RESERVED = 0xAAAAAAAA,
    MANAGER = 0xFFFFFFFF
};

enum class Domain : uint32_t
{
    D0 = 0x00000003,
    D1 = 0x0000000C,
    D2 = 0x00000030,
    D3 = 0x000000C0,
    D4 = 0x00000300,
    D5 = 0x00000C00,
    D6 = 0x00003000,
    D7 = 0x0000C000,
    D8 = 0x00030000,
    D9 = 0x000C0000,
    D10 = 0x00300000,
    D11 = 0x00C00000,
    D12 = 0x03000000,
    D13 = 0x0C000000,
    D14 = 0x30000000,
    D15 = 0xC0000000,
    ALL_DOMAINS = 0xFFFFFFFF
};

void set_DACR(Domain domain, DACRValue access_type);
uint32_t get_DACR();

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

/**
 * Provides information about the implemented memory model and memory management support.
 *
 * More information here -> https://developer.arm.com/docs/ddi0595/e/aarch64-system-registers/id_mmfr0_el1
 */
class MemoryModelFeatures
{
public:
    inline MemoryModelFeatures()
    {
        __asm__ volatile("mrc p15, 0, %[result], c0, c1, 4"
                         : [result] "=r"(m_ID_MMFR0));

        m_inner_shr = (m_ID_MMFR0 >> 28);
        m_FCSE = (m_ID_MMFR0 >> 24) & 0xF;
        m_aux_reg = (m_ID_MMFR0 >> 20) & 0xF;
        m_TCM = (m_ID_MMFR0 >> 16) & 0xF;
        m_share_lvl = (m_ID_MMFR0 >> 12) & 0xF;
        m_outer_shr = (m_ID_MMFR0 >> 8) & 0xF;
        m_PMSA = (m_ID_MMFR0 >> 4) & 0xF;
        m_VMSA = m_ID_MMFR0 & 0xF;
    }

    inline uint32_t ID_MMFR0() { return m_ID_MMFR0; }

    inline uint8_t innerShr() { return m_inner_shr; }
    inline uint8_t FCSE() { return m_FCSE; }
    inline uint8_t auxReg() { return m_aux_reg; }
    inline uint8_t TCM() { return m_TCM; }
    inline uint8_t shareLvl() { return m_share_lvl; }
    inline uint8_t outerShr() { return m_outer_shr; }
    inline uint8_t PMSA() { return m_PMSA; }
    inline uint8_t VMSA() { return m_VMSA; }

private:
    uint32_t m_ID_MMFR0; // 32-bits

    uint8_t m_inner_shr; // 4-bits
    uint8_t m_FCSE;      // 4-bits
    uint8_t m_aux_reg;   // 4-bits
    uint8_t m_TCM;       // 4-bits
    uint8_t m_share_lvl; // 4-bits
    uint8_t m_outer_shr; // 4-bits
    uint8_t m_PMSA;      // 4-bits
    uint8_t m_VMSA;      // 4-bits
};