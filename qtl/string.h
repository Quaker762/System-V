#include <stddef.h>

namespace qtl
{

class String
{
public:
    ~String() {}

    String() {}

    String(const char* cstring);

    String(const char* cstring, size_t length);

private:
    char m_characters[256];
};
}
