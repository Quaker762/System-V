/**
 *  IRQ handler interface
 */
#pragma once

#include <stdint.h>

class IRQHandler
{
public:
    virtual ~IRQHandler();
    virtual void handle_irq() = 0; // This is pure virtual, and needs to be implemented in the driver

protected:
    IRQHandler(uint8_t irq);

private:
    uint8_t m_irq_number { 0 };
};