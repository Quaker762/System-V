/**
 * 
 */
#include <kernel/arch/arm/cpu.h>
#include <kernel/assertions.h>
#include <kernel/device/timer.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/scheduler.h>

//#define SCHED_DEBUG

Process* current_proc;
static Process* s_proc0; // This is the idle kernel process

qtl::CircularLinkedList<Process> g_proc_list;

void Scheduler::initialise()
{
    current_proc = nullptr;

    s_proc0 = Process::spawn_kernel_process("kernel", nullptr);
    g_proc_list.append(s_proc0);
}

void Scheduler::task_switch(RegisterDump& context)
{
    InterruptDisabler disabler;

    if(!current_proc)
        current_proc = s_proc0;

    // This could be a `memcpy()`, but I prefer the verbosity of this
    const_cast<RegisterDump&>(current_proc->registers()).r0 = context.r0;
    const_cast<RegisterDump&>(current_proc->registers()).r1 = context.r1;
    const_cast<RegisterDump&>(current_proc->registers()).r2 = context.r2;
    const_cast<RegisterDump&>(current_proc->registers()).r3 = context.r3;
    const_cast<RegisterDump&>(current_proc->registers()).r4 = context.r4;
    const_cast<RegisterDump&>(current_proc->registers()).r5 = context.r5;
    const_cast<RegisterDump&>(current_proc->registers()).r6 = context.r6;
    const_cast<RegisterDump&>(current_proc->registers()).r7 = context.r7;
    const_cast<RegisterDump&>(current_proc->registers()).r8 = context.r8;
    const_cast<RegisterDump&>(current_proc->registers()).r9 = context.r9;
    const_cast<RegisterDump&>(current_proc->registers()).r10 = context.r10;
    const_cast<RegisterDump&>(current_proc->registers()).fp = context.fp;
    const_cast<RegisterDump&>(current_proc->registers()).r12 = context.r12;
    const_cast<RegisterDump&>(current_proc->registers()).sp = context.sp;
    const_cast<RegisterDump&>(current_proc->registers()).lr = context.lr;
    const_cast<RegisterDump&>(current_proc->registers()).spsr = context.spsr;

    current_proc = const_cast<Process*>(current_proc->next());

    // A magician never reveals his tricks! Well, sometimes he does, and that
    // time is now. This will propogate all the way back to `irq_trampoline`.
    // The side-effect of this is that when we unwind from two calls
    // (irq_handler->task_switch), it'll automatically resume current_proc's
    // state!

    ASSERT(current_proc);

#ifdef SCHEDULE_DEBUG
    kprintf("CONTEXT SWITCH 0x%x (current = %d, next = %d)\n", context.sp, current_proc->pid, current_proc->next->pid);
#endif

    context.r0 = current_proc->registers().r0;
    context.r1 = current_proc->registers().r1;
    context.r2 = current_proc->registers().r2;
    context.r3 = current_proc->registers().r3;
    context.r4 = current_proc->registers().r4;
    context.r5 = current_proc->registers().r5;
    context.r6 = current_proc->registers().r6;
    context.r7 = current_proc->registers().r7;
    context.r8 = current_proc->registers().r8;
    context.r9 = current_proc->registers().r9;
    context.r10 = current_proc->registers().r10;
    context.fp = current_proc->registers().fp;
    context.r12 = current_proc->registers().r12;
    context.sp = current_proc->registers().sp;
    context.lr = current_proc->registers().lr;
    context.spsr = current_proc->registers().spsr;
}
