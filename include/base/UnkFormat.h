#pragma once

// vsnprintf with a bounded result (src/base/UnkFormat.cpp): the formatted length, or when the output
// did not fit, the length of what was written.

#include <stdarg.h>

#include "types.h"

s32 fn_710007ba20(char* out, s32 size, const char* format, va_list args);
