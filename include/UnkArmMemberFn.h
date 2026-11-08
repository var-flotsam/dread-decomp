#pragma once

// Calls a pointer to member function that the game stored, in the ARM C++ ABI's encoding.
//
// Both ABIs keep a member function pointer as two words {ptr, adj}, but they mark a virtual
// function differently. The ARM C++ ABI (AArch64) sets bit 0 of adj: `this` is moved by adj >> 1,
// and ptr is then the byte offset of the function in the vtable. The generic Itanium ABI (x86-64)
// sets bit 0 of ptr instead and moves `this` by adj. A pointer built by the game's code, or
// written in its data, is in the ARM form even on the port: {0x98, 1} is "vtable slot at 0x98",
// which an x86-64 call reads as a function at address 0x98.
//
//   DREAD_CALL_MEMBER(obj, method)(args...)
//
// is `(obj->*method)(args...)` on AArch64, so the matching build's bytes are unchanged; on the
// port it decodes the ARM form. Use it only where the pointer comes from the game (a field, a
// table, a capture); a constant the source itself names, such as a template argument
// `template <void (C::*M)()>`, is encoded by the compiler that calls it and stays `(obj->*M)()`.

#include "types.h"

#if defined(__aarch64__)

#define DREAD_CALL_MEMBER(obj, method) ((obj)->*(method))

#else

namespace dread {

template <class R, class... A>
struct ArmBoundMember {
    void* self;
    R (*fn)(void*, A...);
    R operator()(A... args) const { return fn(self, args...); }
};

template <class C, class R, class... A>
inline ArmBoundMember<R, A...> ArmMember(C* obj, R (C::*method)(A...)) {
    struct Words {
        u64 ptr;
        s64 adj;
    } words;
    static_assert(sizeof(method) == sizeof(words), "a member function pointer is two words");
    __builtin_memcpy(&words, &method, sizeof(words));
    char* self = reinterpret_cast<char*>(obj) + (words.adj >> 1);
    u64 fn = words.ptr;
    if (words.adj & 1)
        fn = *reinterpret_cast<u64*>(*reinterpret_cast<char**>(self) + words.ptr);
    return {self, reinterpret_cast<R (*)(void*, A...)>(fn)};
}

}  // namespace dread

#define DREAD_CALL_MEMBER(obj, method) ::dread::ArmMember((obj), (method))

#endif
