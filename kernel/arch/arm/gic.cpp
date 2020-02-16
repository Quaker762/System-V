/**
 * 
 */
#include <kernel/arch/arm/gic.h>
#include <kernel/kstdlib/kstdio.h>

//#define GIC_DEBUG

// CPU Interface Register offsets
static constexpr uint16_t GICC_CTLR = 0x000;
static constexpr uint16_t GICC_PMR = 0x004;
static constexpr uint16_t GICC_BPR = 0x008;
static constexpr uint16_t GICC_IAR = 0x00c;
static constexpr uint16_t GICC_EOIR = 0x010;
static constexpr uint16_t GICC_RPR = 0x014;
static constexpr uint16_t GICC_HPPIR = 0x018;

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0416b/Beigghcc.html
static constexpr uint16_t GICD_CTLR = 0x000;
static constexpr uint16_t GICD_TYPER = 0x004;
static constexpr uint16_t GICD_ISENABLERn = 0x100;
static constexpr uint16_t GICD_ICENABLERn = 0x180;
static constexpr uint16_t GICD_ISPENDRn = 0x200;
static constexpr uint16_t GICD_ICPENDRn = 0x280;
static constexpr uint16_t GICD_ISACTIVERn = 0x300;
static constexpr uint16_t GICD_IPRIORITYRn = 0x400;
static constexpr uint16_t GICD_ITARGETSRn = 0x800;
static constexpr uint16_t GICD_ICFGRn = 0xc00;
static constexpr uint16_t GICD_SGIR = 0xf00;

static void write_gicc_register(uint16_t reg, uint32_t value)
{
    *reinterpret_cast<uint32_t*>(GIC_CPU_BASE + reg) = value;
}

static uint32_t read_gicc_register(uint16_t reg)
{
    return *reinterpret_cast<uint32_t*>(GIC_CPU_BASE + reg);
}

static void write_gicd_register(uint16_t reg, uint32_t value)
{
    *reinterpret_cast<uint32_t*>(GIC_DIST_BASE + reg) = value;
}

static uint32_t read_gicd_register(uint16_t reg)
{
    return *reinterpret_cast<uint32_t*>(GIC_DIST_BASE + reg);
}

namespace GIC
{

static void set_interrupt_priority(uint16_t irq, uint8_t priority)
{
    uint32_t offset = irq / 4;
    uint32_t index = irq & 3;
    uint32_t val = read_gicd_register(GICD_IPRIORITYRn + (offset * 4));

    val = val | (priority << (8 * index));
    write_gicd_register(GICD_IPRIORITYRn + (offset * 4), val);
}

static void set_interrupt_enabled(uint16_t irq)
{
    uint32_t reg = irq / 32;
    uint32_t bit = irq % 32;

    uint32_t reg_val = read_gicd_register(GICD_ISENABLERn + (reg * 4));
    reg_val |= (1 << bit);
    write_gicd_register(GICD_ISENABLERn + (reg * 4), reg_val);
}

static void set_interrupt_target_cpu(uint16_t irq, uint8_t cpu)
{
    uint32_t offset = irq / 4;
    uint32_t index = irq & 3;
    uint32_t val = read_gicd_register(GICD_ITARGETSRn + (offset * 4));

    val = val | (cpu << (8 * index));
    write_gicd_register(GICD_ITARGETSRn + (offset * 4), val);
}

void initialize()
{
    kprintf("gic: initializing interrupt controller\n");

    write_gicd_register(GICD_CTLR, 0);
    write_gicc_register(GICC_PMR, 0xff);

    *reinterpret_cast<uint32_t*>(GIC_DIST_BASE + GICC_CTLR) |= 1;
    *reinterpret_cast<uint32_t*>(GIC_CPU_BASE + GICD_CTLR) |= 1;
}

uint16_t interrupt_id()
{
    return read_gicc_register(GICC_IAR) & 0x3ff;
}

void enable_interrupt(uint8_t irq)
{
#ifdef GIC_DEBUG
    kprintf("gic: enabling IRQ %d\n");
#endif

    set_interrupt_enabled(irq);
    set_interrupt_priority(irq, 0x80);
    set_interrupt_target_cpu(irq, 1);
}

void disable_interrupt(uint8_t irq)
{
    uint16_t reg = irq / 32;
    uint16_t bit = irq % 32;

#ifdef GIC_DEBUG
    kprintf("gic: disabling IRQ %d\n");
#endif

    uint32_t enbl = read_gicd_register(GICD_ICENABLERn + (4 * reg));
    enbl |= bit;
    write_gicd_register(GICD_ICENABLERn + (4 * reg), enbl);
}

void eoi(uint16_t irq)
{
#ifdef GIC_DEBUG
    kprintf("gic: sending EOI for interrupt %d\n", irq);
#endif
    write_gicc_register(GICC_EOIR, (irq & 0xff));
}

} // namespace GIC