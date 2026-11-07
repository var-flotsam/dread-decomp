#pragma once

#include "types.h"

// The base of an empty tag passed by value in place of an argument register the original leaves as
// it is (a caller that does not set x0, a callee that does not read x1, ...). On AArch64 an empty
// class still takes its register, so the tag keeps every later argument in the original's register
// at no instruction, which is why the matching build uses it. The x86-64 ABI passes no register for
// an empty class: every later argument would move up one register, and a lifted callee, which reads
// its arguments by register position, would take the wrong values. On the port the tag therefore
// holds one byte, which gives it a register of its own.
// Derive the tag from this base: `struct UnkSomethingTag : UnkLeftoverReg {};`.
struct UnkLeftoverReg {
#if !defined(__aarch64__)
    u8 unused;
#endif
};
