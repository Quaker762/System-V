/**
 * 
 */
#pragma once

#include <stddef.h>

void* memcpy(void* src, void* dst, size_t num);
int memcmp(void* src, void* target, size_t num);
void* memset(void* ptr, int value, size_t num);

char* strcpy(char* dst, const char* src);
int strcmp(const char* src, const char* target);
size_t strlen(const char* str);
