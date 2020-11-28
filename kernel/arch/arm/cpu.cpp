#include <kernel/arch/arm/cpu.h>
#include <kernel/assertions.h>

void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func)
{
    //kprintf("\nASSERTION FAILED %s\n%s:%d (%s)\n", msg, file, line, func);
    for(;;)
        ;
}
