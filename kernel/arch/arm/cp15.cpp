#include <kernel/arch/arm/cp15.h>

static inline uint32_t get_DACR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c3, c0, 0"
                     : [result] "=r"(reg));

    return reg;
}

static inline void set_DACR(uint32_t reg)
{
    __asm__ volatile("mcr p15, 0, %[value], c3, c0, 0"
                     :
                     : [value] "r"(reg));
}

static inline uint32_t get_SCTLR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c1, c0, 0"
                     : [result] "=r"(reg));

    return reg;
}

static inline void set_SCTLR(uint32_t reg)
{
    __asm__ volatile("mcr p15, 0, %[value], c1, c0, 0"
                     :
                     : [value] "r"(reg));
}

namespace CP15
{

void set_DACR_by_domain(Domain domain, DACRValue access_type)
{
    uint32_t reg = get_DACR();
    uint32_t mask = static_cast<uint32_t>(domain) & static_cast<uint32_t>(access_type);

    reg &= (~static_cast<uint32_t>(domain)); // Set domain to 00
    reg |= mask;                             // Set domain to specified value

    set_DACR(reg);
}

void set_SCTLR_flag(SCTLRFlag flag)
{
    uint32_t reg = get_SCTLR();

    reg |= static_cast<uint32_t>(flag);

    set_SCTLR(reg);
}

void unset_SCTLR_flag(SCTLRFlag flag)
{
    uint32_t reg = get_SCTLR();

    reg &= ~static_cast<uint32_t>(flag);

    set_SCTLR(reg);
}

bool get_SCTLR_flag(SCTLRFlag flag)
{
    uint32_t reg = get_SCTLR();

    if((reg & static_cast<uint32_t>(flag)) == 0)
        return false;

    return true;
}

void set_TTBR0(uint32_t address)
{
    __asm__ volatile("mcr p15, 0, %[value], c2, c0, 0"
                     :
                     : [value] "r"(address));
}

uint32_t get_TTBR0()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 0"
                     : [result] "=r"(address));

    return address;
}

void set_TTBCR(uint32_t reg)
{
    __asm__ volatile("mrc p15, 0, %[value], c2, c0, 2"
                     :
                     : [value] "r"(reg));
}

uint32_t get_TTBCR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 2"
                     : [result] "=r"(reg));

    return reg;
}

uint32_t get_IFSR()
{
    uint32_t reg;
    __asm__ volatile("mrc p15, 0, %[result], c5, c0, 1"
                     : [result] "=r"(reg));
    return reg;
}

uint32_t get_DFSR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c5, c0, 0"
                     : [result] "=r"(reg));
    return reg;
}

uint32_t get_IFAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 2"
                     : [result] "=r"(address));
    return address;
}

uint32_t get_DFAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 0"
                     : [result] "=r"(address));
    return address;
}

uint32_t get_FAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c6, c0, 1"
                     : [result] "=r"(address));
    return address;
}

uint32_t get_VBAR()
{
    uint32_t address;

    __asm__ volatile("mrc p15, 0, %[result], c12, c0, 0"
                     : [result] "=r"(address));
    return address;
}

void set_VBAR(uint32_t address)
{
    __asm__ volatile("mrc p15, 0, %[value], c2, c0, 2"
                     :
                     : [value] "r"(address));
}
} // namespace CP15
