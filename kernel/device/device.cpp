/**
 * 
 */
#include <kernel/device/device.h>

Device::Device(unsigned int major, unsigned int minor)
: File(Type::DEVICE), m_major(major), m_minor(minor)
{
}

Device::~Device()
{
    // Do something here, I imagine
}