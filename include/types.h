#pragma once

// Fixed-width types without depending on a C/C++ standard library, so the
// same sources build with every candidate toolchain (AArch64 LP64).
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long;
using s8 = signed char;
using s16 = short;
using s32 = int;
using s64 = long;
using f32 = float;
using f64 = double;

static_assert(sizeof(u64) == 8 && sizeof(void*) == 8, "expects AArch64 LP64");
