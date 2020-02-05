/**
 * 
 */
#include <kernel/arch/arm/cpu.h>
#include <kernel/arch/arm/gic.h>
#include <kernel/irqhandler.h>

IRQHandler::IRQHandler(uint8_t irq)
: m_irq_number(irq)
{
    install_handler(irq, *this);
    GIC::enable_interrupt(irq);
}

IRQHandler::~IRQHandler()
{
    remove_handler(m_irq_number);
    GIC::disable_interrupt(m_irq_number);
}