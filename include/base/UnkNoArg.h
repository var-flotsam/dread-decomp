#pragma once

// An argument that takes a register and carries nothing: an empty class passed by value (a tag, a stateless
// functor, an allocator). The AArch64 C++ ABI gives it the next argument register and no value, so a call
// with one leaves that register as it was, which is what the original does at these calls. It is how a
// source says "this register is not loaded" without reading a variable that was never set (CLAUDE.md,
// "No unset values"). The name is a placeholder: the original's tag types are not known.
struct UnkNoArg {};
