/**
 * 
 */
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/vm/physpage.h>

#define DEBUG_PHYSICAL_PAGE

PhysicalMemoryPage& PhysicalMemoryPage::create(PhysicalAddress paddr, Granularity granularity, bool kernel)
{
#ifdef DEBUG_PHYSICAL_PAGE
    const char* gran_string = (granularity == Granularity::PAGE1K) ? "1K" : "4K";
    const char* mode = (kernel == true) ? "Kernel Mode" : "User Mode";

    kprintf("Creating a new %s physical page @ 0x%x for %s mode\n", gran_string, paddr.get(), mode);
#endif

    return *new PhysicalMemoryPage(paddr, granularity, kernel);
}

PhysicalMemoryPage::PhysicalMemoryPage(PhysicalAddress paddr, Granularity granularity, bool kernel)
: m_paddr(paddr), m_granularity(granularity), m_kernel_page(kernel)
{
}
