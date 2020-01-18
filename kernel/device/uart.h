/**
 * System-V UART Driver
 * 
 * Offsets for registers can be found here:
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0159b/I1035564.html
 * http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183f/DDI0183.pdf
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

#define UART0_BASE 0x09000000

class UART
{
public:
    UART(uint32_t base_address);

    void write_string(const char*) const;
    void clear_errors() const;
    void disable() const;
    void enable() const;

    uint16_t flags_register() const { return *reinterpret_cast<volatile uint32_t*>(m_base_address + UARTCR); }
    uint16_t control_register() const { return *reinterpret_cast<volatile uint32_t*>(m_base_address + UARTTFR); }

private:
    void write_character(char) const;
    void write_byte(uint8_t) const;

private:
    uint32_t m_base_address { 0 };
};
