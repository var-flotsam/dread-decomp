#pragma once

// A string (base::global::CRntString, 0x30 bytes) followed by its own inline storage: an
// allocator with a vtable, N bytes, a word and a count. It is built empty on that storage
// (fn_710007e6e4), then assigned a C string (fn_710007e77c) or another string (fn_710007eaa0),
// and destroyed in two steps (fn_710007ed40, fn_710007ecd4); base/global/CRntString.h declares
// those. Names and roles are placeholders.

#include "base/global/CRntString.h"
#include "types.h"

template <u32 N, void** Vtable>
struct UnkBufferString {
    u64 field_0x0[6];
    struct {
        void** vtable;
        u8 data[N];
    } storage;
    u64 field_tail;
    u32 field_tail_count;

    // The string part (not a member: the string's constructor and destructor run by hand).
    __attribute__((always_inline)) base::global::CRntString* Str() {
        return reinterpret_cast<base::global::CRntString*>(this);
    }
    __attribute__((always_inline)) const base::global::CRntString* Str() const {
        return reinterpret_cast<const base::global::CRntString*>(this);
    }

    __attribute__((always_inline)) void Init() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = Vtable + 2;
        field_tail = 0;
        field_tail_count = 0;
    }

    __attribute__((always_inline)) UnkBufferString() { Init(); }
    __attribute__((always_inline)) explicit UnkBufferString(const char* str) {
        Init();
        fn_710007e77c(Str(), str);
    }
    __attribute__((always_inline)) UnkBufferString(const UnkBufferString& other) {
        Init();
        fn_710007eaa0(Str(), other.Str());
    }
    __attribute__((always_inline)) ~UnkBufferString() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};
