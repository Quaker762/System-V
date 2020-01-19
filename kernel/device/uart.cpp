/**
 * 
 */
#include "kernel/device/uart.h"

#include <common/assert.h>

UART::UART(uint32_t base_address)
: m_base_address(base_address)
{
}

void UART::write_character(char character) const
{
    ASSERT(m_base_address);
    //while(status_register() & UARTFR_BUSY); // This seems to break gdb! Let's not do it, shall we!
    *reinterpret_cast<volatile uint32_t*>(m_base_address) = character;

    // Add a return carriage if we detect a newline
    if(character == '\n')
        *reinterpret_cast<volatile uint32_t*>(m_base_address) = '\r';
}

void UART::write_string(const char* string) const
{
    while(*string)
        write_character(*string++);
}

void UART::puts(const char* string) const
{
    write_string(string);
}

void UART::putc(char character) const
{
    write_character(character);
}