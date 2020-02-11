/**
 * File base class
 * 
 * EVERYTHING in the UNIX file system (including devices!) are considered files.
 */
#pragma once

#include <kernel/fs/filedescription.h>
#include <stddef.h>
#include <stdint.h>

class FileDescription;

class File
{
public:
    enum class Type
    {
        NONE,
        DEVICE,
        DIRECTORY,
        FILE,
        PROC,
        LINK
    };

    // FIXME: Open should actually return a FileDescription/FileDescriptor object that actually tracks
    // the state of the File Descriptor, can seek it etc
protected:
    virtual ~File();
    virtual int open(uint32_t) = 0;
    virtual int close() = 0;
    virtual size_t write(FileDescription&, size_t size, size_t pos) = 0;
    virtual size_t read(FileDescription&, size_t size, size_t pos) = 0;
    //void int ioctl() // What the fuck is the prototype for this?!!

    Type type() const { return m_type; }
    bool is_device() const { return m_type == Type::DEVICE; }

protected:
    File(Type);

protected:
    Type m_type { Type::NONE };
};