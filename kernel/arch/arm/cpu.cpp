/**
 *
 */
#include "arch/arm/cpu.h"
#include "assertions.h"

void __assertion_failed(const char*, const char*, unsigned, const char*)
{
    for(;;)
        ;
}
