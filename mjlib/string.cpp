#include "string.h"

namespace MJ
{

String::String(const char* cstring)
{
    char character = 0;
    for(int i = 0; character != '\0'; ++i)
    {
        character = *(cstring + i);
        m_characters[i] = character;
    }
}

String::String(const char* cstring, size_t length)
{
    for(int i = 0; i < static_cast<int>(length); ++i)
    {
        m_characters[i] = *(cstring + i);
    }
}
}
