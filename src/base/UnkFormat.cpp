// vsnprintf with a bounded result (unit 55, 0x710007b430-0x710007bb30): the formatted length, or
// when the output did not fit, the length of what was written (the buffer is terminated). Nothing is
// written without a buffer, a positive size and a format. Written by hand: its callers pass a va_list,
// which on the port must be the host's (a lift would read an AArch64 one).
#include <stdio.h>
#include <string.h>

#include "base/UnkFormat.h"

// NON_MATCHING(minor): the original copies the va_list's four words to its frame before the size test;
// same behaviour.
s32 fn_710007ba20(char* out, s32 size, const char* format, va_list args) {
    s32 n = 0;
    if (size > 0 && out && format) {
        n = vsnprintf(out, size, format, args);
        if (n < 0) {
            out[size - 1] = 0;
            return (s32)strlen(out);
        }
    }
    return n;
}
