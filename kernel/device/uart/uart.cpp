/**
 * 
 */
#include <common/assert.h>
#include <kernel/device/uart/uart.h>

UART::UART(uint32_t base_address)
: m_base_address(base_address)
{
}

void UART::write_character(char character) const
{
    ASSERT(m_base_address);
    while(control_register() & UARTFR_TXFF)
        ;
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
