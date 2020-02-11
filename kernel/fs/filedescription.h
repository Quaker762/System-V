/**
 *
 */
#pragma once

#include <kernel/fs/file.h>
#include <stddef.h>
#include <stdint.h>

typedef size_t offset_t;
class File;

class FileDescription
{
public:
    FileDescription& create();
    FileDescription& create(File&);

    int close();

    offset_t seek(offset_t, int whence);
    size_t read(uint8_t*, size_t);
    size_t write(const uint8_t*, size_t);

    ~FileDescription();

private:
    FileDescription();
    FileDescription(File&);

    // We also need an inode here too
private:
    File* m_file;

    offset_t m_offset;
    uint32_t m_flags;
};