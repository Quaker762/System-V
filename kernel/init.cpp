/**
 *  https://github.com/qemu/qemu/blob/master/hw/arm/versatilepb.c
 * 
 * Note: The -serial mon:stdio option for QEMU multiplexes the Monitor and Serial
 * port into stdio. To switch between them, type Ctrl+a, then 'c'.
 */
extern "C" int init(void)
{
    for(;;) {}

    return 0;
}
