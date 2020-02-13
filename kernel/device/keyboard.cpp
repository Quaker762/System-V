#include <kernel/device/keyboard.h>
#include <kernel/kstdlib/kstdio.h>

static constexpr char SCAN_CODE_PRIMARY_MAP[0x84] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\t',
    '`',
    0,
    0, // 0x10
    0,
    0,
    0,
    0,
    'q',
    '1',
    0,
    0,
    0,
    'z',
    's',
    'a',
    'w',
    '2',
    0,
    0, // 0x20
    'c',
    'x',
    'd',
    'e',
    '4',
    '3',
    0,
    0,
    ' ',
    'v',
    'f',
    't',
    'r',
    '5',
    0,
    0, // 0x30
    'n',
    'b',
    'h',
    'g',
    'y',
    '6',
    0,
    0,
    0,
    'm',
    'j',
    'u',
    '7',
    '8',
    0,
    0, // 0x40
    ',',
    'k',
    'i',
    'o',
    '0',
    '9',
    0,
    0,
    '.',
    '/',
    'l',
    ';',
    'p',
    '-',
    0,
    0, // 0x50
    0,
    '\'',
    0,
    '[',
    '=',
    0,
    0,
    0,
    0,
    '\n',
    ']',
    0,
    '\\',
    0,
    0,
    0, // 0x60
    0,
    0,
    0,
    0,
    0,
    0x08, // Backspace
    0,
    0,
    '1', // Keypad
    0,
    '4', // Keypad
    '7', //Keypad
    0,
    0,
    0,
    '0',  // 0x70 Keypad
    '.',  // Keypad
    '2',  // Keypad
    '5',  // Keypad
    '6',  // Keypad
    '8',  // Keypad
    0x1b, //escape
    0,
    0,
    '+', // Keypad
    '3', // Keypad
    '-', // Keypad
    '*', //Keypad
    '9',
    0,
    0,
    0, // 0x80
    0,
    0,
    0
};

static constexpr char SHIFTED_SCAN_CODE_PRIMARY_MAP[0x84] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\t',
    '~',
    0,
    0, // 0x10
    0,
    0,
    0,
    0,
    'Q',
    '!',
    0,
    0,
    0,
    'Z',
    'S',
    'A',
    'W',
    '@',
    0,
    0, // 0x20
    'C',
    'X',
    'D',
    'E',
    '$',
    '#',
    0,
    0,
    ' ',
    'V',
    'F',
    'T',
    'R',
    '%',
    0,
    0, // 0x30
    'N',
    'B',
    'H',
    'G',
    'Y',
    '^',
    0,
    0,
    0,
    'M',
    'J',
    'U',
    '&',
    '*',
    0,
    0, // 0x40
    '<',
    'K',
    'I',
    'O',
    ')',
    '(',
    0,
    0,
    '>',
    '?',
    'L',
    ':',
    'P',
    '_',
    0,
    0, // 0x50
    0,
    '"',
    0,
    '{',
    '+',
    0,
    0,
    0,
    0,
    '\n',
    '}',
    0,
    '|',
    0,
    0,
    0, // 0x60
    0,
    0,
    0,
    0,
    0,
    0x08, // Backspace
    0,
    0,
    '1', // Keypad
    0,
    '4', // Keypad
    '7', //Keypad
    0,
    0,
    0,
    '0',  // 0x70 Keypad
    '.',  // Keypad
    '2',  // Keypad
    '5',  // Keypad
    '6',  // Keypad
    '8',  // Keypad
    0x1b, //escape
    0,
    0,
    '+', // Keypad
    '3', // Keypad
    '-', // Keypad
    '*', //Keypad
    '9',
    0,
    0,
    0, // 0x80
    0,
    0,
    0
};

static constexpr char CAPSLOCK_SCAN_CODE_PRIMARY_MAP[0x84] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\t',
    '`',
    0,
    0, // 0x10
    0,
    0,
    0,
    0,
    'Q',
    '1',
    0,
    0,
    0,
    'Z',
    'S',
    'A',
    'W',
    '2',
    0,
    0, // 0x20
    'C',
    'X',
    'D',
    'E',
    '4',
    '3',
    0,
    0,
    ' ',
    'V',
    'F',
    'T',
    'R',
    '5',
    0,
    0, // 0x30
    'N',
    'B',
    'H',
    'G',
    'Y',
    '6',
    0,
    0,
    0,
    'M',
    'J',
    'U',
    '7',
    '8',
    0,
    0, // 0x40
    ',',
    'K',
    'I',
    'O',
    '9',
    '0',
    0,
    0,
    '.',
    '/',
    'L',
    ';',
    'P',
    '-',
    0,
    0, // 0x50
    0,
    '\'',
    0,
    '[',
    '=',
    0,
    0,
    0,
    0,
    '\n',
    ']',
    0,
    '\\',
    0,
    0,
    0, // 0x60
    0,
    0,
    0,
    0,
    0,
    0x08, // Backspace
    0,
    0,
    '1', // Keypad
    0,
    '4', // Keypad
    '7', //Keypad
    0,
    0,
    0,
    '0',  // 0x70 Keypad
    '.',  // Keypad
    '2',  // Keypad
    '5',  // Keypad
    '6',  // Keypad
    '8',  // Keypad
    0x1b, //escape
    0,
    0,
    '+', // Keypad
    '3', // Keypad
    '-', // Keypad
    '*', //Keypad
    '9',
    0,
    0,
    0, // 0x80
    0,
    0,
    0
};
static constexpr char SCAN_CODE_SECONDARY_MAP[0x7d] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 0x10
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 0x20
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 0x30
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 0x40
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '/',
    0,
    0,
    0,
    0,
    0,
    0, // 0x50
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\n',
    0,
    0,
    0,
    0,
    0,
    0, // 0x60
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,    // 0x70
    0x7f, // delete
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

// Multi-byte signals
static constexpr char SECONDARY_SIGNAL = 0xe0;
static constexpr char RELEASE_SIGNAL = 0xf0;

// Primary modifier keys
static constexpr char LALT = 0x11;
static constexpr char LSHIFT = 0x12;
static constexpr char LCTRL = 0x14;
static constexpr char CAPSLOCK = 0x58;
static constexpr char RSHIFT = 0x59;
//static constexpr char NUMLOCK = 0x77;

// Secondary modifier keys
static constexpr char RALT = 0x11;
static constexpr char RCTRL = 0x14;
static constexpr char LGUI = 0x1f;
static constexpr char RGUI = 0x27;

void Keyboard::handle_irq(RegisterDump&)
{
    // Wait for the reciever register to populate before reading
    while((register_read(KMISTAT) & static_cast<uint8_t>(KeyboardStatusBit::RXFULL)) == 0) {}
    uint8_t input = register_read(KMIDATA);

    // Check for multi-byte signals
    switch(input)
    {
        case RELEASE_SIGNAL:
            m_release_signal = true;
            return;
        case SECONDARY_SIGNAL:
            m_secondary_signal = true;
            return;
        default:
            break;
    }

    char ascii_out = 0;

    if(!m_secondary_signal && !m_release_signal) // Process primary key press
    {
        switch(input)
        {
            case LALT:
                m_lalt = true;
                return;
            case LSHIFT:
                m_lshift = true;
                return;
            case LCTRL:
                m_lctr = true;
                return;
            case CAPSLOCK:
                m_capslock = !m_capslock;
                return;
            case RSHIFT:
                m_rshift = true;
                return;
            default:
                break;
        }

        // Shift takes priority here
        if(m_lshift || m_rshift)
        {
            ascii_out = SHIFTED_SCAN_CODE_PRIMARY_MAP[input];
        }
        else if(m_capslock)
        {
            ascii_out = CAPSLOCK_SCAN_CODE_PRIMARY_MAP[input];
        }
        else
        {
            ascii_out = SCAN_CODE_PRIMARY_MAP[input];
        }
    }
    else if(m_secondary_signal && !m_release_signal) // Process secondary key press
    {
        m_secondary_signal = false;
        switch(input)
        {
            case RALT:
                m_ralt = true;
                return;
            case RCTRL:
                m_rctr = true;
                return;
            case LGUI:
                m_lgui = !m_lgui;
                return;
            case RGUI:
                m_rgui = !m_rgui;
                return;
            default:
                break;
        }
        ascii_out = SCAN_CODE_SECONDARY_MAP[input];
    }
    else if(m_release_signal && !m_secondary_signal) // Process primary key release
    {
        m_release_signal = false;
        switch(input)
        {
            case LALT:
                m_lalt = false;
                return;
            case LSHIFT:
                m_lshift = false;
                return;
            case LCTRL:
                m_lctr = false;
                return;
            case RSHIFT:
                m_rshift = false;
                return;
            default:
                break;
        }
    }
    else if(m_release_signal && m_secondary_signal) // Process secondary key release
    {
        m_secondary_signal = false;
        m_release_signal = false;
        switch(input)
        {
            case RALT:
                m_ralt = false;
                return;
            case RCTRL:
                m_rctr = false;
                return;
            default:
                break;
        }
    }

    if(ascii_out)
        kprintf("%c", ascii_out);
}

void Keyboard::enable() const
{
    set_control_bit(KeyboardControlBit::KMIEN);
    set_control_bit(KeyboardControlBit::KMIRXINTREN);
}

void Keyboard::disable() const
{
    unset_control_bit(KeyboardControlBit::KMIEN);
    unset_control_bit(KeyboardControlBit::KMIRXINTREN);
}

int Keyboard::open(uint32_t)
{
    return 0;
}

int Keyboard::close()
{
    return 0;
}

size_t Keyboard::write(FileDescription&, size_t, size_t)
{
    return 0;
}

size_t Keyboard::read(FileDescription&, size_t, size_t)
{
    return 1;
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
