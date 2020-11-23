/**
 * 
 */
#pragma once

#include <kernel/mem/address.h>

class PhysicalMemoryPage
{
public:
    enum Granularity
    {
        PAGE4K,
        PAGE1K
    };

public:
    ~PhysicalMemoryPage();
    PhysicalAddress paddr() const { return m_paddr; }
    Granularity granularity() const { return m_granularity; };

    static PhysicalMemoryPage& create(PhysicalAddress paddr, Granularity granularity, bool kernel);

private:
    PhysicalMemoryPage(PhysicalAddress paddr, Granularity granularity, bool kernel);

    PhysicalAddress m_paddr;
    Granularity m_granularity;
    bool m_kernel_page { false };
};
