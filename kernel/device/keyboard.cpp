#include <kernel/device/keyboard.h>
#include <kernel/kstdlib/kstdio.h>

void Keyboard::handle_irq()
{
    // Wait for the reciever register to populate before reading
    while(register_read(KMISTAT) & static_cast<uint8_t>(KeyboardStatusBit::RXFULL))
    {
        kprintf("Button Pressed: %d\n", register_read(KMIDATA));
    }
}

void Keyboard::enable() const
{
    set_control_bit(KeyboardControlBit::KMIEN);
    set_control_bit(KeyboardControlBit::KMIRXINTREN);
}

///////////////////////////////////////////////

void Keyboard::register_write(uint8_t reg, uint8_t value) const
{
    *reinterpret_cast<uint32_t*>(PL050_BASE_ADDRESS + reg) = value;
}

uint8_t Keyboard::register_read(uint8_t reg) const
{
    return *reinterpret_cast<uint32_t*>(PL050_BASE_ADDRESS + reg);
}

void Keyboard::set_control_bit(KeyboardControlBit bit) const
{
    uint8_t reg = register_read(KMICR);
    reg |= static_cast<uint8_t>(bit);
    register_write(KMICR, reg);
}

void Keyboard::unset_control_bit(KeyboardControlBit bit) const
{
    uint8_t reg = register_read(KMICR);
    reg &= ~static_cast<uint8_t>(bit);
    register_write(KMICR, reg);
}

bool Keyboard::read_status_bit(KeyboardStatusBit bit) const
{
    uint8_t reg = register_read(KMICR);

    if((reg & static_cast<uint8_t>(bit)) == 0)
        return false;

    return true;
}