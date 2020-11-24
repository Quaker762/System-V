/**
 * 
 */
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/vm/physpage.h>

#define DEBUG_PHYSICAL_PAGE

PhysicalMemoryPage* PhysicalMemoryPage::create(PhysicalAddress paddr, Granularity granularity, bool kernel, bool can_free)
{
#ifdef DEBUG_PHYSICAL_PAGE
    const char* gran_string = (granularity == Granularity::PAGE1K) ? "1K" : "4K";
    const char* mode = (kernel == true) ? "Kernel Mode" : "User Mode";
    const char* free = (can_free == true) ? "can be freed" : "cannot be freed";

    kprintf("Creating a new %s physical page @ 0x%x for %s mode\n", gran_string, paddr.get(), mode);
    kprintf("This page %s\n", free);
#endif

    return new PhysicalMemoryPage(paddr, granularity, kernel, can_free);
}

PhysicalMemoryPage::PhysicalMemoryPage(PhysicalAddress paddr, Granularity granularity, bool kernel, bool can_free)
: m_paddr(paddr),
  m_granularity(granularity),
  m_kernel_page(kernel),
  m_can_free(can_free)
{
}
