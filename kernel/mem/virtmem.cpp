/**
 * 
 */
#include <kernel/device/uart.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/address.h>
#include <kernel/mem/virtmem.h>

extern uint32_t __RAM_START;
extern uint32_t __RAM_END;
extern uint32_t __FREE_RAM_START;

VirtMemoryManager::VirtMemoryManager()
{
    L1TranslationTable* l1_table_base = allocate_L1_translation_table();
    for(uint32_t i = 0; i < ENTIRES_PER_L1_TRANSLATION_TABLE; ++i)
    {
        l1_table_base->entries[i].entry = 0;
    }

    L2TranslationTable* l2_table_base;
    uint32_t free_ram_start = reinterpret_cast<uint32_t>(&__FREE_RAM_START);
    uint32_t ram_start = reinterpret_cast<uint32_t>(&__RAM_START);
    uint32_t kernel_size = free_ram_start - ram_start;
    uint32_t blocks = kernel_size / 0x1000;
    uint32_t kernel_page = ram_start;

    for(uint32_t i = 2048; i <= (2048 + (blocks / 256)); ++i)
    {
        l1_table_base->entries[i].entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_1k_page()) | 0x00000001;
        l2_table_base = reinterpret_cast<L2TranslationTable*>(l1_table_base->entries[i].table_base_address << 10);

        for(uint32_t j = 0; j < 256 && kernel_page < free_ram_start; ++j)
        {
            l2_table_base->entries[j].entry = kernel_page;
            l2_table_base->entries[j].always_1 = 0b1;
            l2_table_base->entries[j].ap_lo = 0b11;
            kernel_page += 0x1000;
        }
    }

    l1_table_base->entries[448].entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_1k_page()) | 0x00000001;
    l2_table_base = reinterpret_cast<L2TranslationTable*>(l1_table_base->entries[448].table_base_address << 10);
    for(uint32_t j = 0; j < 256; ++j)
    {
        l2_table_base->entries[j].entry = 0x1c000000 + (j * 0x1000);
        l2_table_base->entries[j].always_1 = 0b1;
        l2_table_base->entries[j].ap_lo = 0b11;
    }

    l1_table_base->entries[704].entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_1k_page()) | 0x00000001;
    l2_table_base = reinterpret_cast<L2TranslationTable*>(l1_table_base->entries[704].table_base_address << 10);
    for(uint32_t j = 0; j < 256; ++j)
    {
        l2_table_base->entries[j].entry = 0x2c000000 + (j * 0x1000);
        l2_table_base->entries[j].always_1 = 0b1;
        l2_table_base->entries[j].ap_lo = 0b11;
    }

    CPU::set_TTBR0(l1_table_base);
    CPU::invalidate_unified_TLB();
    CPU::set_DACR_by_domain(Domain::ALL_DOMAINS, DACRValue::CLIENT);
    CPU::set_SCTLR_flag(SCTLRFlag::MMU_ENABLE);
}

void VirtMemoryManager::initialise_L1_translation_table(L1TranslationTable& table)
{
    for(uint32_t i = 0; i < ENTIRES_PER_L1_TRANSLATION_TABLE; ++i)
    {
        table.entries[i].entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_1k_page());
        table.entries[i].always_1 = 0b1;
        table.entries[i].always_0 = 0b0;
        table.entries[i].pxn = 0b0;
        table.entries[i].ns = 0b0;
        table.entries[i].sbz = 0b0;
        table.entries[i].domain = 0b0000;
        table.entries[i].unused = 0b0;
    }
}

void VirtMemoryManager::initialise_L2_translation_table(L2TranslationTable& table)
{
    for(uint32_t i = 0; i < ENTIRES_PER_L2_TRANSLATION_TABLE; ++i)
    {
        table.entries[i].entry = reinterpret_cast<uint32_t>(PhysicalMemoryManager::obj_instance().allocate_physical_page());
        table.entries[i].xn = 0b0;
        table.entries[i].always_1 = 0b1;
        table.entries[i].b = 0b0;
        table.entries[i].c = 0b0;
        table.entries[i].ap_lo = 0b11;
        table.entries[i].tex = 0b000;
        table.entries[i].ap_hi = 0b0;
        table.entries[i].s = 0b0;
        table.entries[i].nG = 0b0;
    }
}
