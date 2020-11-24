/**
 * 
 */
#pragma once

#include <kernel/mem/address.h>
#include <kernel/mem/physmem.h>

class PhysicalmemoryManager;

class PhysicalMemoryPage
{
    friend class PhysicalMemoryManager;
    friend class PageDirectory;

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

    static PhysicalMemoryPage* create(PhysicalAddress paddr, Granularity granularity = Granularity::PAGE4K, bool kernel = true, bool can_free = false);

    bool can_be_freed() const { return m_can_free; }

private:
    PhysicalMemoryPage(PhysicalAddress paddr, Granularity granularity, bool kernel, bool can_free);

    PhysicalAddress m_paddr;
    Granularity m_granularity;
    bool m_kernel_page { false };
    bool m_can_free { false };
};
