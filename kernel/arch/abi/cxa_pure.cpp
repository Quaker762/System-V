/**
 * Function required by gcc to use/enable pure virtual functions. 
 * 
 * A pure virtual function is a function that requires an implementation
 * by a derived class.
 */
#include "arch/arm/cpu.h"

extern "C" void __cxa_pure_virtual()
{
    // This should NEVER happen. According to the osdev wiki
    // These functions should actually never be called, because without hacks, or 
    // through undefined behaviour of your kernel, it is not possible to instantiate 
    // a class that doesn't define all pure virtual functions. 


    // TODO: Print an error message here
    halt();
}