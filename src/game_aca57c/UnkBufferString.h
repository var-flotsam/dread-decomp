#pragma once

// A string (a 0x30-byte header) followed by its own inline storage: a vtable, N bytes, a word and
// a count. It is built empty on that storage (fn_710007e6e4), then assigned a C string
// (fn_710007e77c) or another string (fn_710007eaa0), and destroyed in two steps (fn_710007ed40,
// fn_710007ecd4). Names and roles are placeholders.

#include "types.h"

void fn_710007e6e4(void* self, void* inline_buffer);
void fn_710007e77c(void* self, const char* str);
void fn_710007eaa0(void* self, const void* other);
void fn_710007ed40(void* self);
void fn_710007ecd4(void* self);

template <u32 N, void** Vtable>
struct UnkBufferString {
    u64 field_0x0[6];
    struct {
        void** vtable;
        u8 data[N];
    } storage;
    u64 field_tail;
    u32 field_tail_count;

    __attribute__((always_inline)) void Init() {
        fn_710007e6e4(this, &storage);
        storage.vtable = Vtable + 2;
        field_tail = 0;
        field_tail_count = 0;
    }

    __attribute__((always_inline)) UnkBufferString() { Init(); }
    __attribute__((always_inline)) explicit UnkBufferString(const char* str) {
        Init();
        fn_710007e77c(this, str);
    }
    __attribute__((always_inline)) UnkBufferString(const UnkBufferString& other) {
        Init();
        fn_710007eaa0(this, &other);
    }
    __attribute__((always_inline)) ~UnkBufferString() {
        fn_710007ed40(this);
        fn_710007ecd4(this);
    }
};
