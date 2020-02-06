/**
 * 
 */
#pragma once

#include <kernel/irqhandler.h>
#include <stdint.h>

static constexpr uint8_t KMI0_IRQ = 44;
static constexpr uint8_t KMI1_IRQ = 45;

static constexpr uintptr_t PL050_BASE_ADDRESS = 0x1c060000; // 0x1c070000

static constexpr uint8_t KMICR = 0x00;     // Control register
static constexpr uint8_t KMISTAT = 0x04;   // Status register
static constexpr uint8_t KMIDATA = 0x08;   // Received data/Data to be transmitted
static constexpr uint8_t KMICLKDIV = 0x0c; // Clock divisor register
static constexpr uint8_t KMIIR = 0x10;     // Interrupt status register

enum class KeyboardControlBit : uint8_t
{
    FKMIC = 0x01,
    FKMID = 0x02,
    KMIEN = 0x04,
    KMITXINTREN = 0x08,
    KMIRXINTREN = 0x10,
    KMITYPE = 0x20
};

enum class KeyboardStatusBit : uint8_t
{
    KMID = 0x01,
    KMIC = 0x02,
    RXPARITY = 0x04,
    RXBUSY = 0x08,
    RXFULL = 0x10,
    TXBUSY = 0x20,
    TXEMPTY = 0x40
};

enum class KeyboardInterruptIDBit : uint8_t
{
    KMIRXINTR = 0x01,
    KMITXINTR = 0x02
};

class Keyboard : public IRQHandler
{
public:
    Keyboard()
    : IRQHandler(KMI0_IRQ) {}
    void enable() const;

    void handle_irq();

private:
    void register_write(uint8_t, uint8_t) const;
    uint8_t register_read(uint8_t) const;

    void set_control_bit(KeyboardControlBit) const;
    void unset_control_bit(KeyboardControlBit) const;

    bool read_status_bit(KeyboardStatusBit) const;
};