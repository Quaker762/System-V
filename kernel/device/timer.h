/**
 * 
 */
#pragma once

#include <kernel/irqhandler.h>
#include <stdint.h>

#define TIMER_LOAD 0x00
#define TIMER_VAL 0x04
#define TIMER_CTL 0x08
#define TIMER_INT_CLR 0x0c
#define TIMER_RIS 0x10
#define TIMER_MIS 0x14
#define TIMER_BG_LOAD 0x18

static constexpr uint8_t TIM01_IRQ = 34;
static constexpr uint8_t TIM23_IRQ = 35;

enum class TimerBaseAdddress : uintptr_t
{
    TIMER0 = 0x1c110000,
    TIMER1 = 0x10010020,
    TIMER2 = 0x1c120000,
    TIMER3 = 0x1c120020
};

enum class TimerControlBits : uint8_t
{
    ONESHOT = 0x01,
    SIZE = 0x02,
    PRE = 0x0C,
    RSVD = 0x10,
    INT_ENBL = 0x20,
    MODE = 0x40,
    ENABLE = 0x80
};

class Timer : public IRQHandler
{
public:
    Timer(TimerBaseAdddress);

    void load(uint32_t) const;
    void enable();
    void disable();
    void clear_interrupt();

    uint32_t read_value() const;
    uint8_t control() const;

    void handle_irq(RegisterDump&);

private:
    void register_write(uint8_t, uint32_t) const;
    uint32_t register_read(uint8_t) const;

    void set_control_bit(TimerControlBits) const;
    void unset_control_bit(TimerControlBits) const;

    const char* timer_name() const;

private:
    bool m_enabled { false };
    uintptr_t m_base_address { 0 };
};
