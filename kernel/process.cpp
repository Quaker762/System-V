/**
 * 
 */
#include <kernel/kstdlib/kstring.h>
#include <kernel/mem/heap/heap.h>
#include <kernel/process.h>

Process* Process::spawn_kernel_process(const char* name, proc_fn code)
{
    return new Process(PrivilegeMode::KERNEL_MODE);
}

Process* Process::spawn_user_process(const char* name)
{
    return new Process(PrivilegeMode::USER_MODE);
}

Process::Process(PrivilegeMode priv)
: m_privilege(priv)
{
    memset(&m_regs, 0, sizeof(Registers));
}

Process::Process(PrivilegeMode priv, proc_fn entry)
: m_privilege(priv)
{
    memset(&m_regs, 0, sizeof(Registers));
    m_regs.pc = reinterpret_cast<uint32_t>(entry); // Should we set lr instead???
    m_regs.sp = reinterpret_cast<uint8_t>(&m_regs + PROC_STACK_SIZE - 1);
}