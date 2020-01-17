/**
 * 
 */
#pragma once

#include "arch/arm/cpu.h"

[[noreturn]]void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func);

#define ASSERT(expr) \
    if(!static_cast<bool>(expr)) \
     __assertion_failed(#  expr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define ASSERT_NOT_REACHED() ASSERT(false)