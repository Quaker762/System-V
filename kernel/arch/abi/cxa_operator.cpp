/**
 *  We don't use `new` just yet (because we have no Kernel heap allocator),
 *   
 */

void operator delete(void*, unsigned int)
{
    // Do nothing for now
}

void operator delete(void*)
{
    // Do nothing for now
}