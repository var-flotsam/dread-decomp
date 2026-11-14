// nn::atk-ish unit 2772: small accessors on packed parameter blocks, a flag word followed by optional
// values. Names and layouts are placeholders.
#include "game/UnkAtkSystem.h"
#include "game/UnkTU2772.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

s64 fn_710122fdf4(s32 n) {
    return (s64)n * 12 + (((s64)n << 8) | 0x40);
}

u32 fn_7101213700(s32 n) {
    if (n == 2)
        return 3;
    return n == 1 ? 2 : 1;
}

void fn_710122fef4(UnkAtkCounter_b0147* self) {
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x10 = 0;
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    __atomic_store_n(&self->field_0x24, 0, __ATOMIC_RELEASE);
}

u32 fn_71012105c4(u8* self) {
    u32* p = (u32*)(self + 0xc);
    u32 f = *p;
    if (!(f & 2))
        return 0x40;
    return p[1 + (f & 1)];
}

u32 fn_710121060c(u8* self) {
    u32* p = (u32*)(self + 0x4);
    u32 f = *p;
    if (!(f & 2))
        return 0;
    return p[1 + (f & 1)];
}

u32 fn_710121062c(u8* self) {
    u32* p = (u32*)(self + 0x8);
    u32 f = *p;
    if (!(f & 2))
        return 0;
    return p[1 + (f & 1)];
}

f32 fn_71012307f8(u8* self) {
    u32* p = (u32*)self;
    u32 f = *p;
    if (!(f & 2))
        return 1.0f;
    return ((f32*)p)[1 + (f & 1)];
}

u32 fn_7101230a40(u8* self) {
    u32* p = (u32*)(self + 0x4);
    u32 f = *p;
    if (!(f & 2))
        return 0x60;
    return p[1 + (f & 1)];
}

u8 fn_7101210278(u8* self) {
    u32* p = (u32*)(self + 0x14);
    u32 f = *p;
    if (!(f & 4))
        return 0x40;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx];
}

u32 fn_71012309ac(u8* self) {
    u32* p = (u32*)(self + 0x0);
    u32 f = *p;
    if (!(f & 4))
        return 0x40;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx];
}

u32 fn_7101230a60(u8* self) {
    u32* p = (u32*)(self + 0x4);
    u32 f = *p;
    if (!(f & 4))
        return 0x40;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx];
}

u8 fn_71012102a0(u8* self) {
    u32* p = (u32*)(self + 0x14);
    u32 f = *p;
    if (!(f & 4))
        return 0;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx] >> 8;
}

u32 fn_71012309d4(u8* self) {
    u32* p = (u32*)(self + 0x0);
    u32 f = *p;
    if (!(f & 4))
        return 0;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx] >> 8;
}

u32 fn_7101230a88(u8* self) {
    u32* p = (u32*)(self + 0x4);
    u32 f = *p;
    if (!(f & 4))
        return 0;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return p[1 + idx] >> 8;
}

struct UnkAtkClearList_b0147 {
    u8 pad_0x0[8];
    u32* count;  // 0x8
    u64* data;  // 0x10
    u8 hasCount;  // 0x18
};

void fn_7101213168(UnkAtkClearList_b0147* self) {
    for (u64 i = 0; i < (self->hasCount ? *self->count : 0); i++)
        __builtin_memset(&self->data[i], 0, sizeof(u64));
}

struct UnkAtkLevel_b0147 {
    u8 pad_0x0[0x1e8];
    u16 flags;  // 0x1e8
    u8 mode;  // 0x1ea
    u8 pad_0x1eb[0x15];
    f32 level;  // 0x200
};

void fn_710122d7f0(UnkAtkLevel_b0147* self, f32 level, u32 mode) {
    f32 v = !(level < 0.0f) ? level : 0.0f;
    if (level > 1.0f)
        v = 1.0f;
    bool changed = false;
    if (self->mode != mode) {
        self->mode = (u8)mode;
        changed = true;
    }
    if (v != self->level) {
        self->level = v;
        changed = true;
    }
    if (changed)
        self->flags |= 0x20;
}

struct UnkAtkListLink_b0147 {
    UnkAtkListLink_b0147* prev;
    UnkAtkListLink_b0147* next;
};

struct UnkAtkRegistry_b0147 {
    u8 field_0x0;
    UnkAtkListLink_b0147 first;  // 0x8
    UnkAtkListLink_b0147 second;  // 0x18
};

struct UnkAtkRegistryGuarded_b0147 {
    u64 guard;
    UnkAtkRegistry_b0147 value;  // 0x8
};
extern UnkAtkRegistryGuarded_b0147 g_unk_7172a185e0 __attribute__((visibility("hidden")));

UnkAtkRegistry_b0147* fn_710122edb0() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7172a185e0.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_7172a185e0.guard)) {
            UnkAtkRegistry_b0147* r = &g_unk_7172a185e0.value;
            r->first.prev = r->first.next = &r->first;
            r->field_0x0 = 0;
            r->second.prev = r->second.next = &r->second;
            __cxa_guard_release(&g_unk_7172a185e0.guard);
        }
    }
    return &g_unk_7172a185e0.value;
}
