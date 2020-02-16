/**
 * 
 */
#pragma once

#include <kernel/arch/arm/cpu.h>
#include <mjlib/extra.h>
#include <stddef.h>
#include <stdint.h>

static constexpr size_t PROC_STACK_SIZE = (4 * KiB) / sizeof(uint32_t);
typedef void (*proc_fn)();

class Process
{
public:
    enum PrivilegeMode
    {
        KERNEL_MODE,
        USER_MODE
    };

public:
    static Process* spawn_kernel_process(const char* name, proc_fn code);
    static Process* spawn_user_process(const char* name); // This should get some kind of ELF handle???

    Process(PrivilegeMode);
    Process(PrivilegeMode, proc_fn);
    ~Process();

    const RegisterDump& registers();
    uint32_t pid();

    const Process* next() const;
    void set_next(Process*);

private:
    uint32_t m_kstack[PROC_STACK_SIZE]; // uint32_t for alignment!
    PrivilegeMode m_privilege;
    Process* m_next;
    uint32_t m_pid;
    RegisterDump m_registers;
};
