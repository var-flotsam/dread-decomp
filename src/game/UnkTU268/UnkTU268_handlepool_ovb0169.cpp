// Unit 268 (no class hint): a manager of pooled handles: small pools of preallocated handles and the list of the
// ones handed out to owners. Names and layouts are placeholders.
#include "types.h"

struct UnkHandle;
void fn_710015c1e0(UnkHandle** out, s32 width, s32 height, s32 count, const s32& format);
void fn_710015c250(char* p0);
void fn_71001e9f40(void* self);

struct UnkHandleUse_b0169 {
    UnkHandle* handle;
    void* owner;
};

struct UnkHandleSlotsB_b0169 {
    u8 pad_0x0[8];
    UnkHandle* slots[10];  // 0x8
    u32 used;  // 0x58

    // Gives a handle back, swapping it behind the handed-out ones.
    __attribute__((always_inline)) void Release(UnkHandle* h) {
        u32 n = used;
        if (n) {
            u64 left = n;
            UnkHandle** p = slots;
            for (; left; --left, ++p) {
                if (*p == h) {
                    used = --n;
                    UnkHandle* last = slots[n];
                    // Untyped store: the original reloads the count after it.
                    __builtin_memcpy(p, &last, sizeof(last));
                    __builtin_memcpy(&slots[used], &h, sizeof(h));
                    break;
                }
            }
        }
    }
};

struct UnkHandlePool_b0169 {
    u8 pad_0x0[8];
    UnkHandle* slots[4];  // 0x8
    u32 used;  // 0x28
    u8 pad_0x2c[4];
    UnkHandleSlotsB_b0169 pool_b;  // 0x30
    UnkHandleUse_b0169 uses[4];  // 0x90
    u32 use_count;  // 0xd0
    u8 pad_0xd4[4];
    UnkHandleUse_b0169 uses_b[10];  // 0xd8
    u32 use_count_b;  // 0x178
    bool flag;  // 0x17c

    // Gives the handle of use `i` back to the slots, swapping it behind the handed-out ones.
    __attribute__((always_inline)) void Release(UnkHandle* h) {
        u32 n = used;
        if (n) {
            u64 left = n;
            UnkHandle** p = slots;
            for (; left; --left, ++p) {
                if (*p == h) {
                    used = --n;
                    UnkHandle* last = slots[n];
                    // Untyped store: the original reloads the count after it.
                    __builtin_memcpy(p, &last, sizeof(last));
                    __builtin_memcpy(&slots[used], &h, sizeof(h));
                    break;
                }
            }
        }
    }

    __attribute__((always_inline)) void ReleaseAll() {
        for (u64 i = 0; i < use_count; ++i) {
            Release(uses[i].handle);
            __builtin_memset(&uses[i], 0, sizeof(uses[i]));
        }
    }
};
static_assert(__builtin_offsetof(UnkHandlePool_b0169, flag) == 0x17c, "layout");

void fn_71001e9ffc(UnkHandlePool_b0169* self, bool flag) {
    self->ReleaseAll();
    self->use_count = 0;
    self->flag = flag;
}

void fn_71001e9e54(UnkHandlePool_b0169* self) {
    self->ReleaseAll();
    self->use_count = 0;
    for (int i = 0; i < 4; ++i) {
        if (self->slots[i]) {
            fn_710015c250(reinterpret_cast<char*>(self->slots[i]));
            self->slots[i] = nullptr;
        }
    }
    fn_71001e9f40(&self->pool_b);
}

struct UnkFormatArg_b0169 {
    s32 value;
    s32 unused;
};

struct UnkHandleSet4_b0169 {
    u8 pad_0x0[8];
    UnkHandle* slots[4];  // 0x8
};

struct UnkHandleSet10_b0169 {
    u8 pad_0x0[8];
    UnkHandle* slots[10];  // 0x8
};

// Creates the four large handles the set hands out; the ones that fail are skipped.
void fn_71001e9b54(UnkHandleSet4_b0169* self) {
    u32 count = 0;
    for (int i = 0; i < 4; ++i) {
        UnkHandle* h;
        UnkFormatArg_b0169 format;
        format.value = 0x12;
        fn_710015c1e0(&h, 0x800, 0x800, 0xc, format.value);
        if (h)
            self->slots[count++] = h;
    }
}

// The same for ten smaller handles.
void fn_71001e9c44(UnkHandleSet10_b0169* self) {
    u32 count = 0;
    for (int i = 0; i < 10; ++i) {
        UnkHandle* h;
        UnkFormatArg_b0169 format;
        format.value = 0x12;
        fn_710015c1e0(&h, 0x200, 0x200, 0xc, format.value);
        if (h)
            self->slots[count++] = h;
    }
}
