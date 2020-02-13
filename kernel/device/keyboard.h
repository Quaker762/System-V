/**
 * 
 */
#pragma once

#include <kernel/device/chardevice.h>
#include <kernel/irqhandler.h>
#include <kernel/mem/heap/heap.h>
#include <stdint.h>

static constexpr uint8_t KMI0_IRQ = 44;
//static constexpr uint8_t KMI1_IRQ = 45;

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

class Keyboard : public IRQHandler, public CharacterDevice
{
    OBJ_PERMANENT
public:
    Keyboard()
    : IRQHandler(KMI0_IRQ), CharacterDevice(1, 0)
    {
        m_instance = this;
        this->enable();
    }
    void enable() const;
    void disable() const;

    void handle_irq(RegisterDump&);

    int open(uint32_t) override;
    int close() override;
    size_t write(FileDescription&, size_t, size_t) override;
    size_t read(FileDescription&, size_t size, size_t pos) override;

private:
    void register_write(uint8_t, uint8_t) const;
    uint8_t register_read(uint8_t) const;

    void set_control_bit(KeyboardControlBit) const;
    void unset_control_bit(KeyboardControlBit) const;

    bool read_status_bit(KeyboardStatusBit) const;

    const Keyboard& obj_instance() const { return *m_instance; }

private:
    bool m_capslock { false };
    bool m_lshift { false };
    bool m_rshift { false };
    bool m_lctr { false };
    bool m_rctr { false };
    bool m_lgui { false };
    bool m_rgui { false };
    bool m_lalt { false };
    bool m_ralt { false };

    // Currently we ignore scroll lock and assume numlock is always on
    //bool m_scrllock { false };
    //bool m_numlock { false };

    bool m_secondary_signal { false };
    bool m_release_signal { false };

    Keyboard* m_instance { nullptr };
};
