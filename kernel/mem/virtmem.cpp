/**
 * 
 */
#include <kernel/arch/arm/gic.h>
#include <kernel/device/keyboard.h>
#include <kernel/device/timer.h>
#include <kernel/device/uart.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/kstdlib/kstring.h>
#include <kernel/mem/address.h>
#include <kernel/mem/virtmem.h>

extern uint32_t __RAM_START;
extern uint32_t __RAM_END;
extern uint32_t __FREE_RAM_START;
static constexpr uint32_t PHYS_PAGE_SIZE = 0x1000;
static constexpr uint32_t L1_TABLE_ENTRY_DEFAULT = 0x00000001;
static constexpr uint32_t L2_TABLE_ENTRY_DEFAULT = 0x00000032;

VirtMemoryManager::VirtMemoryManager()
{
    /* First we build the translation table to simply map the kernel and devices */
    m_current_table = allocate_L1_translation_table();
    memset(m_current_table, 0, sizeof(L1TranslationTable));
    identity_map_kernel();
    map_devices();

    /* Now we can use this table as our map and enable the MMU */
    switch_L1_translation_table(m_current_table);
    CPU::invalidate_unified_TLB();
    CPU::set_DACR_by_domain(Domain::ALL_DOMAINS, DACRValue::CLIENT);
    CPU::set_SCTLR_flag(SCTLRFlag::MMU_ENABLE);
}

void VirtMemoryManager::identity_map_kernel()
{
    PhysicalAddress free_ram_start(reinterpret_cast<uintptr_t>(&__FREE_RAM_START));
    PhysicalAddress ram_start(reinterpret_cast<uintptr_t>(&__RAM_START));

    PhysicalAddress phys = ram_start;
    VirtualAddress virt(reinterpret_cast<uintptr_t>(&__RAM_START));
    int kernel_pages = (free_ram_start.get() - ram_start.get()) / PHYS_PAGE_SIZE;
    for(int i = 0; i < kernel_pages; phys += PHYS_PAGE_SIZE, virt += PHYS_PAGE_SIZE, ++i)
    {
        map_address(phys, virt);
    }
}

void VirtMemoryManager::map_devices()
{
    map_address(PhysicalAddress(PL050_BASE_ADDRESS), VirtualAddress(PL050_BASE_ADDRESS));
    map_address(PhysicalAddress(GIC_BASE), VirtualAddress(GIC_BASE));
    map_address(PhysicalAddress(GIC_DIST_BASE), VirtualAddress(GIC_DIST_BASE));
    map_address(PhysicalAddress(GIC_CPU_BASE), VirtualAddress(GIC_CPU_BASE));
    map_address(PhysicalAddress(UART0_BASE), VirtualAddress(UART0_BASE));
    map_address(PhysicalAddress(static_cast<uintptr_t>(TimerBaseAddress::TIMER0)), VirtualAddress(static_cast<uintptr_t>(TimerBaseAddress::TIMER0)));
}

void VirtMemoryManager::map_address(PhysicalAddress phys_addr, VirtualAddress virt_addr)
{
    L1TableEntry* l1_entry = &m_current_table->entries[l1_translation_table_index(virt_addr)];
    L2TranslationTable* l2_table;

    /* Check if table exists */
    if(!l1_entry->entry)
    {
        l2_table = reinterpret_cast<L2TranslationTable*>(PhysicalMemoryManager::obj_instance().allocate_1k_page());
        memset(l2_table, 0, sizeof(L2TranslationTable));
        l1_entry->entry = reinterpret_cast<uint32_t>(l2_table) | L1_TABLE_ENTRY_DEFAULT;
    }
    else
    {
        l2_table = reinterpret_cast<L2TranslationTable*>(l1_entry->table_base_address << 10);
    }

    L2TableEntry* l2_entry = &l2_table->entries[l2_translation_table_index(virt_addr)];
    l2_entry->entry = phys_addr.get() | L2_TABLE_ENTRY_DEFAULT;
}

void VirtMemoryManager::allocate_page(L2TableEntry& entry)
{
    entry.entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_physical_page()) | L2_TABLE_ENTRY_DEFAULT;
}

void VirtMemoryManager::free_page(L2TableEntry& entry)
{
    PhysicalMemoryManager::obj_instance().free_page(reinterpret_cast<void*>(entry.page_base_address << 10));
    entry.entry = 0;
}