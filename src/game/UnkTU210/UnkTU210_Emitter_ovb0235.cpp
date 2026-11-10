// A unit with no known name (tools/tu_map.py unit 210, 0x710016e75c-0x7100170300): a GPU particle
// system's emitters. fn_7100170184 ages an emitter's particles (the ones whose age has reached their
// lifetime are swapped out of the live range) and hands its two buffers back to the free list;
// fn_710016e910 creates the 0x400 resource slots and their handles. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern u32 g_unk_716e352430;
extern void* g_unk_716e34e430[];

struct UnkEmitterChild_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(u32* indices, u32 count);
};

struct UnkEmitter_b0235 {
    u8 pad_0x0[0x40];
    f32* start_times;  // 0x40
    u8 pad_0x48[0x10];
    f32 default_life;  // 0x58
    u8 pad_0x5c[4];
    f32* lifetimes;  // 0x60
    u8 pad_0x68[0x210 - 0x68];
    UnkEmitterChild_b0235** children;  // 0x210
    u8 pad_0x218[4];
    u32 child_count;  // 0x21c
    u64 field_0x220;
    u64 field_0x228;
    u8 pad_0x230[0x370 - 0x230];
    f32 time;  // 0x370
    u8 pad_0x374[0x390 - 0x374];
    u32 count;  // 0x390
    u8 pad_0x394[4];
    u32* indices;  // 0x398
    void* field_0x3a0;
    void* field_0x3a8;
};
static_assert(__builtin_offsetof(UnkEmitter_b0235, field_0x3a8) == 0x3a8, "UnkEmitter_b0235 layout");

// Ages the particles; the ones whose age has reached their lifetime are swapped out of the live range.
static inline __attribute__((always_inline)) void Age(UnkEmitter_b0235* self, f32 dt) {
    self->time += dt;
    u32 remaining = self->count;
    u32 i = 0;
    while (remaining) {
        u32* indices = self->indices;
        --remaining;
        u32 particle = indices[i];
        const f32* life = self->lifetimes ? self->lifetimes + particle : &self->default_life;
        f32 limit = *life;
        f32 age = self->time - self->start_times[particle];
        if (!(limit <= age)) {
            ++i;
        } else {
            u32 last = --self->count;
            u32 moved = indices[last];
            // Untyped stores: the original reloads the emitter's fields after them.
            __builtin_memcpy(&indices[i], &moved, sizeof(u32));
            __builtin_memcpy(&self->indices[self->count], &particle, sizeof(u32));
        }
    }
}

static inline __attribute__((always_inline)) void Release(void*& slot) {
    if (slot) {
        u32 n = g_unk_716e352430;
        g_unk_716e34e430[n] = slot;
        g_unk_716e352430 = n + 1;
    }
}

void fn_7100170184(UnkEmitter_b0235* self, f32 dt) {
    Age(self, dt);
    Release(self->field_0x3a0);
    self->field_0x3a0 = nullptr;
    Release(self->field_0x3a8);
    self->field_0x3a8 = nullptr;
    self->field_0x220 = 0;
    self->field_0x228 = 0;
}


// ---- the resource slots


struct UnkObj_7100161b90;
struct UnkGfxBuffer252;

struct UnkSlot_b0235 {
    UnkGfxBuffer252* resource;
    s32 slot;  // 0x8
    u32 field_0xc;
};

extern UnkSlot_b0235* g_unk_716e34e420;
extern UnkSlot_b0235* g_unk_716e34c420[];
extern u32 g_unk_716e34e428;

void fn_7100161960(UnkObj_7100161b90** out, u32 size, void* a, const u32* kind);
void fn_7100161d70(UnkGfxBuffer252* resource);
u32 fn_71001622f0(u32 a, u32 b);
void fn_7100161e60(UnkGfxBuffer252* resource, u32 a, u32 b, u32 c, u32 d, const void* data, u32 flags, u32 e);
void fn_7100161dc0(UnkGfxBuffer252* resource);

void fn_710016e910() {
    u8 zeros[0x24000];
    alignas(8) u32 kind;
    __builtin_memset(zeros, 0, sizeof(zeros));
    UnkSlot_b0235* slots = static_cast<UnkSlot_b0235*>(fn_7100083ba0(0x4000, 0x30));
    for (UnkSlot_b0235 *p = slots, *end = slots + 0x400; p != end; ++p) {
        p->resource = nullptr;
        p->slot = -1;
    }
    g_unk_716e34e420 = slots;
    UnkSlot_b0235** table = g_unk_716e34c420;
    for (u64 i = 0; i != 0x400; ++i) {
        kind = 5;
        fn_7100161960(reinterpret_cast<UnkObj_7100161b90**>(&g_unk_716e34e420[i].resource), 0x24000, nullptr, &kind);
        fn_7100161d70(g_unk_716e34e420[i].resource);
        {
            UnkGfxBuffer252* slot = g_unk_716e34e420[i].resource;
            fn_7100161e60(slot, 0, 0x24, 0, fn_71001622f0(3, 3), zeros, 0xc000, 0);
        }
        {
            UnkGfxBuffer252* slot = g_unk_716e34e420[i].resource;
            fn_7100161e60(slot, 2, 0x24, 0xc, fn_71001622f0(2, 3), nullptr, 0x8000, 0);
        }
        {
            UnkGfxBuffer252* slot = g_unk_716e34e420[i].resource;
            fn_7100161e60(slot, 5, 0x24, 0x14, fn_71001622f0(4, 3), nullptr, 0x10000, 0);
        }
        fn_7100161dc0(g_unk_716e34e420[i].resource);
        // An untyped store: the original reloads the slot array's pointer after it.
        s32 none = -1;
        __builtin_memcpy(&g_unk_716e34e420[i].slot, &none, sizeof(none));
        *table++ = &g_unk_716e34e420[i];
    }
    g_unk_716e34e428 = 0;
}
