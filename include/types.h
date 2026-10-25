#pragma once

// Fixed-width types without depending on a C/C++ standard library, so the
// same sources build with every candidate toolchain. The matching build is
// AArch64 LP64 (`long` is 64-bit); a 64-bit Windows host (LLP64) gets the
// same widths from `long long`, so the port can compile these sources too.
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using s8 = signed char;
using s16 = short;
using s32 = int;
#if defined(_WIN64)
using u64 = unsigned long long;
using s64 = long long;
#else
using u64 = unsigned long;
using s64 = long;
#endif
using f32 = float;
using f64 = double;

static_assert(sizeof(u64) == 8 && sizeof(void*) == 8, "expects a 64-bit target");
