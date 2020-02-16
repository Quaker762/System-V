/**
 * 
 */
#pragma once

#include <kernel/process.h>

extern Process* current_proc;
extern Process* proc_list_head;
extern Process* proc_list_tail;

namespace Scheduler
{

void initialise();

void task_switch(RegisterDump& regs);

}