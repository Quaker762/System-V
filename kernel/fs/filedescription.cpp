/**
 *
 */
#include <kernel/fs/filedescription.h>

FileDescription& FileDescription::create()
{
    return *new FileDescription();
}

FileDescription& FileDescription::create(File& file)
{
    return *new FileDescription(file);
}

FileDescription::FileDescription()
: m_file(nullptr)
{
}

FileDescription::FileDescription(File& file)
: m_file(&file)
{
}