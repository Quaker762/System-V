/**
 * 
 */
#include <kernel/kstdlib/kstdio.h>
#include <kernel/kstdlib/kstring.h>
#include <kernel/mem/heap/heap.h>
#include <kernel/process.h>
#include <kernel/scheduler.h>

static uint32_t GLOBAL_PID = 0;

Process* Process::spawn_kernel_process(const char*, proc_fn entry)
{
    return new Process(PrivilegeMode::KERNEL_MODE, entry);
}

Process* Process::spawn_user_process(const char*)
{
    return new Process(PrivilegeMode::USER_MODE);
}

Process::Process(PrivilegeMode priv)
: m_privilege(priv),
  m_next(nullptr),
  m_pid(GLOBAL_PID++)

{
    //memset(&m_regs, 0, sizeof(Registers));
}

Process::Process(PrivilegeMode priv, proc_fn entry)
: m_privilege(priv),
  m_next(nullptr),
  m_pid(GLOBAL_PID++)
{
    memset(&m_registers, 0, sizeof(RegisterDump));
    m_registers.lr = reinterpret_cast<uint32_t>(entry);
    m_registers.sp = reinterpret_cast<uint32_t>(&m_kstack) + PROC_STACK_SIZE;

    proc_list_tail->m_next = this;
    proc_list_tail = this;
    m_next = proc_list_head;

    kprintf("process: creating a new process with entry point 0x%x (this=0x%x)\n", entry, this);
    kprintf("process: m_kstack base @ 0x%x, top = 0x%x\n", m_kstack, m_registers.sp);
}

const RegisterDump& Process::registers()
{
    return m_registers;
}

uint32_t Process::pid()
{
    return m_pid;
}

const Process* Process::next() const
{
    return m_next;
}

void Process::set_next(Process* proc)
{
    m_next = proc;
}