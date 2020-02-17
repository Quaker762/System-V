/**
 * 
 */
#pragma once

#include <kernel/process.h>

extern Process* current_proc;

namespace Scheduler
{

void initialise();

void task_switch(RegisterDump& regs);

} // namespace Scheduler
