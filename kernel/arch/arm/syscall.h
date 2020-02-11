/**
 * 
 */
#pragma once

#define syscall(arg1, arg2, arg3, arg4, syscall_number) \
    __asm__ volatile("mov %%r0, %0" ::"r"(arg1));       \
    __asm__ volatile("mov %%r1, %0" ::"r"(arg2));       \
    __asm__ volatile("mov %%r2, %0" ::"r"(arg3));       \
    __asm__ volatile("mov %%r3, %0" ::"r"(arg4));       \
    __asm__ volatile("svc %0" ::"i"(syscall_number))
