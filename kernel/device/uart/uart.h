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

#define UARTTRSR 0x04
#define UARTTECR 0x04
#define UARTTFR 0x18
#define UARTIPLR 0x20
#define UARTIBRD 0x24
#define UARTFBRD 0x28
#define UARTLCR_H 0x2c
#define UARTCR 0x30
#define UARTIFLS 0x34
#define UARTIMSC 0x38
#define UARTRIS 0x3c
#define UARTMIS 0x40
#define UARTICR 0x44
#define UARTDMACR 0x48

#define UARTFR_CTS 0x01
#define UARTFR_DSR 0x02
#define UARTFR_DCD 0x04
#define UARTFR_BUSY 0x08
#define UARTFR_RXFE 0x10
#define UARTFR_TXFF 0x20
#define UARTFR_RXFF 0x40
#define UARTFR_TXFE 0x80
#define UARTFR_RI 0x100

#define UARTCR_UARTEN 0x01
#define UARTCR_TXE 0x100
#define UARTCR_RXE 0x200

/*  0xd01f1000 UART0.  */
/*  0xd01f2000 UART1.  */
/*  0xd01f3000 UART2.  */
class UART
{
public:
    static constexpr uintptr_t UART0_BASE = 0xd01f1000;
    static constexpr uintptr_t UART1_BASE = 0xd01f2000;
    static constexpr uintptr_t UART2_BASE = 0xd01f3000;

public:
    UART(uint32_t base_address);

    void puts(const char*) const;
    void putc(char) const;
    void clear_errors() const;
    void disable() const;
    void enable() const;

    uint16_t flags_register() const { return *reinterpret_cast<volatile uint32_t*>(m_base_address + UARTCR); }
    uint16_t control_register() const { return *reinterpret_cast<volatile uint32_t*>(m_base_address + UARTTFR); }

private:
    void write_character(char) const;
    void write_byte(uint8_t) const;
    void write_string(const char*) const;

private:
    uint32_t m_base_address { 0 };
};
