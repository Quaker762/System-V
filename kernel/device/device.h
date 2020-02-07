/**
 * 
 */
#pragma once

#include <kernel/fs/file.h>

class Device : public File
{
public:
    virtual ~Device() override;

protected:
    Device(unsigned int, unsigned int);

    unsigned int major() const { return m_major; }
    unsigned int minor() const { return m_minor; }

private:
    unsigned int m_major;
    unsigned int m_minor;
};