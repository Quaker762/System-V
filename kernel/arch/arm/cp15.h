/**
 * System Control coprocessor (CP15) related functions and classes
 */
#pragma once

#include <kernel/arch/arm/cpu.h>
#include <stdint.h>

/**
 * Provides information about the CPU model.
 */
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
    inline uint32_t MID() { return m_MID; }

    inline uint8_t implementer() { return m_implementer; }
    inline uint8_t variant() { return m_variant; }
    inline uint8_t arch() { return m_arch; }
    inline uint16_t primary_part_numbe() { return m_primary_part_number; }
    inline uint8_t rev() { return m_rev; }

private:
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

enum class SCTLRFlag : uint32_t
{
    MMU_ENABLE = 0x00000001,
    ALIGNMENT_CHECK_ENABLE = 0x00000002,
    CACHE_ENABLE = 0x00000004,
    BRANCH_PREDICTION_ENABLE = 0x00000800,
    INSTRUCTION_CACHE_ENABLE = 0x00001000,
    EXCEPTION_VECTOR_TABLE_BASE = 0x00002000,
    FIQ_CONFIGURATION_ENABLE = 0x00200000,
    ALIGNMENT_MODEL = 0x00400000,
    EXCEPTION_ENDIANNESS = 0x02000000,
    NMFI_SUPPORT = 0x08000000,
    THUMB_EXCEPTION_ENABLE = 0x40000000
};

namespace CPU
{
void set_DACR_by_domain(Domain domain, DACRValue access_type);

void set_SCTLR_flag(SCTLRFlag flag);
void unset_SCTLR_flag(SCTLRFlag flag);
bool get_SCTLR_flag(SCTLRFlag flag);

//static inline void set_TTBR0(L1TranslationTable* address)
//{
//    __asm__ volatile("mcr p15, 0, %[value], c2, c0, 0"
//                     :
//                     : [value] "r"(address));
//}

static inline uint32_t get_TTBR0()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 0"
                     : [result] "=r"(address));

    return address;
}

static inline void invalidate_unified_TLB()
{
    __asm__ volatile("mcr p15, 0, r0, c8, c7, 0");
}

/**
 * Currently we will just want to set this to 0x00000000, as we only plan on using TTBR0
 * Page 1725 of the manual shows the layout of this register
 */
static inline void set_TTBCR(uint32_t reg)
{
    __asm__ volatile("mrc p15, 0, %[value], c2, c0, 2"
                     :
                     : [value] "r"(reg));
}

static inline uint32_t get_TTBCR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 2"
                     : [result] "=r"(reg));

    return reg;
}

static inline uint32_t get_IFSR()
{
    uint32_t reg;
    __asm__ volatile("mrc p15, 0, %[result], c5, c0, 1"
                     : [result] "=r"(reg));
    return reg;
}

static inline uint32_t get_DFSR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c5, c0, 0"
                     : [result] "=r"(reg));
    return reg;
}

static inline uint32_t get_IFAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 2"
                     : [result] "=r"(address));
    return address;
}

static inline uint32_t get_DFAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 0"
                     : [result] "=r"(address));
    return address;
}

static inline uint32_t get_FAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 1"
                     : [result] "=r"(address));
    return address;
}

static inline uint32_t get_VBAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c12, c0, 0"
                     : [result] "=r"(address));
    return address;
}

extern "C" void set_VBAR(uint32_t address); // This can't be inlined because it's used in an assembly stub!

} // namespace CPU
