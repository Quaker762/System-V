/*
 * Copyright (c) 2020 Jesse Buhagiar <jooster669@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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

    bool operator+=(const uint32_t& rhs) { return m_addr += rhs; }

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

    bool operator+=(const uint32_t& rhs) { return m_addr += rhs; }

    uint16_t page_directory_index() const { return m_addr >> 20; }
    uint8_t page_table_index() const { return (m_addr >> 12) & 0xff; }
    uint16_t page_offset() const { return m_addr & 0xfff; }

    bool is_l1_table_aligned() const { return (m_addr & 0x3fff) == 0; }
    bool is_l2_table_aligned() const { return (m_addr & 0x400) == 0; }

private:
    uintptr_t m_addr { 0 };
};
