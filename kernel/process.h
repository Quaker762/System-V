/**
 * 
 */
#pragma once

#include <kernel/arch/arm/cpu.h>
#include <mjlib/extra.h>
#include <stddef.h>
#include <stdint.h>

static constexpr size_t PROC_STACK_SIZE = 8 * KB;
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

private:
    Registers m_regs;
    uint8_t m_stack[PROC_STACK_SIZE];

    PrivilegeMode m_privilege { KERNEL_MODE };
};