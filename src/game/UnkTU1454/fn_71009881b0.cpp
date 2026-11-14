#include "game/CAbilityShinespark.h"
#include "game/UnkTU365.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_b0248() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkSlotElem_b0248 {
    u8 unk_0x0[0x48];
};

struct UnkSlotArray_b0248 {
    u32 count;
    UnkSlotElem_b0248 items[16];
};

struct UnkSlotList_b0248 {
    u8 unk_0x0[8];
    UnkSlotArray_b0248 slots;  // 0x8
    f32 plane_x;  // 0x48c
    f32 plane_y;
    f32 plane_z;
    f32 plane_d;
};

struct UnkDir_b0248 {
    u8 unk_0x0[0x20];
    f32 x, y, z;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkVisitResult_b0040 fn_71009881b0(UnkSlotList_b0248* self, UnkDir_b0248* dir) {
    if (!(dir->x * self->plane_x + dir->y * self->plane_y + dir->z * self->plane_z >= self->plane_d))
        return Skip_b0248();
    s32 room = 16 - self->slots.count;
    if (room > 0) {
        // The record visited is copied into the free slot.
        fn_7100331c7c(&self->slots.items[self->slots.count], dir);
        self->slots.count++;
        return UnkVisitResult_b0040(room == 1 ? 3 : 1);
    }
    return UnkVisitResult_b0040(2);
}
