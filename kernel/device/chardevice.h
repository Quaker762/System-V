/**
 * 
 */
#pragma once

#include <kernel/device/device.h>

class CharacterDevice : public Device
{
public:
    ~CharacterDevice() override;

protected:
    CharacterDevice(unsigned int major, unsigned int minor)
    : Device(major, minor)
    {
    }
};