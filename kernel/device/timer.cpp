/**
 * 
 */
#include <kernel/assertions.h>
#include <kernel/device/timer.h>
#include <kernel/kstdlib/kstdio.h>

#define TIMER_DEBUG

const char* Timer::timer_name() const
{
    TimerBaseAdddress addr = static_cast<TimerBaseAdddress>(m_base_address);

    switch(addr)
    {
        case TimerBaseAdddress::TIMER0:
            return "TIMER0";
        case TimerBaseAdddress::TIMER1:
            return "TIMER1";
        case TimerBaseAdddress::TIMER2:
            return "TIMER2";
        case TimerBaseAdddress::TIMER3:
            return "TIMER3";
    }

    return "UNDEFINED";
}

Timer::Timer(TimerBaseAdddress timer)
: IRQHandler((timer == TimerBaseAdddress::TIMER0 || timer == TimerBaseAdddress::TIMER1) ? TIM01_IRQ : TIM23_IRQ),
  m_base_address(static_cast<uintptr_t>(timer))
{
    kprintf("timer: creating timer with base address of 0x%x\n", static_cast<uint32_t>(timer));
}

void Timer::load(uint32_t value) const
{
#ifdef TIMER_DEBUG
    kprintf("timer: loading timer %s with 0x%x\n", timer_name(), value);
#endif
    register_write(TIMER_LOAD, value);
    register_write(TIMER_BG_LOAD, value);
}

uint32_t Timer::read_value() const
{
    return register_read(TIMER_VAL);
}

uint8_t Timer::control() const
{
    return register_read(TIMER_CTL);
}

void Timer::enable()
{
#ifdef TIMER_DEBUG
    kprintf("timer: enabling timer %s\n", timer_name());
#endif
    m_enabled = true;
    set_control_bit(TimerControlBits::ENABLE);
    set_control_bit(TimerControlBits::INT_ENBL);
    set_control_bit(TimerControlBits::MODE);
}

void Timer::disable()
{
#ifdef TIMER_DEBUG
    kprintf("timer: disabling timer %s\n", timer_name());
#endif
    m_enabled = false;
    unset_control_bit(TimerControlBits::ENABLE);
}

void Timer::handle_irq()
{
    kprintf("Tick!\n");
}

////////////////////////////////////////////////////////////////////////

void Timer::register_write(uint8_t reg, uint32_t value) const
{
    ASSERT(m_base_address);
    *reinterpret_cast<uint32_t*>(m_base_address + reg) = value;
}

uint32_t Timer::register_read(uint8_t reg) const
{
    ASSERT(m_base_address);
    return *reinterpret_cast<uint32_t*>(m_base_address + reg);
}

void Timer::set_control_bit(TimerControlBits bit) const
{
    uint8_t ctl = register_read(TIMER_CTL);

    ctl |= static_cast<uint8_t>(bit);
    register_write(TIMER_CTL, ctl);
}

void Timer::unset_control_bit(TimerControlBits bit) const
{
    uint8_t ctl = register_read(TIMER_CTL);

    ctl &= ~static_cast<uint8_t>(bit);
    register_write(TIMER_CTL, ctl);
}