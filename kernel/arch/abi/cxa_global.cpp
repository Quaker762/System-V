/**
 * Initialises all global constructors by calling function pointers that are located
 * in the .init_array* sections in the ELF binary
 */
extern "C"
{
    typedef void (*ctor)(void);
    extern ctor __init_array_start[0], __init_array_end[0];
    extern ctor __preinit_array_start[0], __preinit_array_end[0];

    void __cxa_global_constructors(void)
    {
        for(ctor* func = __preinit_array_start; func != __preinit_array_end; func++)
            (*func)();

        for(ctor* func = __init_array_start; func != __init_array_end; func++)
            (*func)();
    }
}