/**
 * 
 */
#pragma once

#include <stdint.h>

// Descibes an address that is ALWAYS physical
class PhysicalAddress
{
public:
    explicit PhysicalAddress(uintptr_t addr)
    : m_addr(addr) {}

    uintptr_t get() const { return m_addr; };
    PhysicalAddress offset(uintptr_t offset) { return PhysicalAddress(m_addr + offset); }
    uint8_t* as_ptr() { return reinterpret_cast<uint8_t*>(m_addr); }

    bool operator==(const PhysicalAddress& rhs) { return m_addr == rhs.m_addr; }
    bool operator!=(const PhysicalAddress& rhs) { return m_addr != rhs.m_addr; }
    bool operator>(const PhysicalAddress& rhs) { return m_addr > rhs.m_addr; }
    bool operator<(const PhysicalAddress& rhs) { return m_addr < rhs.m_addr; }

    bool is_l1_table_aligned() const { return (m_addr & 0x3fff) == 0; }
    bool is_l2_table_aligned() const { return (m_addr & 0x400) == 0; }

private:
    uintptr_t m_addr { 0 };
};

// Descibes an address that is ALWAYS virtual
class VirtualAddress
{
public:
    explicit VirtualAddress(uintptr_t addr)
    : m_addr(addr) {}

    uintptr_t get() const { return m_addr; };
    VirtualAddress offset(uintptr_t offset) { return VirtualAddress(m_addr + offset); }
    uint8_t* as_ptr() { return reinterpret_cast<uint8_t*>(m_addr); }

    bool operator==(const VirtualAddress& rhs) { return m_addr == rhs.m_addr; }
    bool operator!=(const VirtualAddress& rhs) { return m_addr != rhs.m_addr; }
    bool operator>(const VirtualAddress& rhs) { return m_addr > rhs.m_addr; }
    bool operator<(const VirtualAddress& rhs) { return m_addr < rhs.m_addr; }

    bool is_l1_table_aligned() const { return (m_addr & 0x3fff) == 0; }
    bool is_l2_table_aligned() const { return (m_addr & 0x400) == 0; }

private:
    uintptr_t m_addr { 0 };
};