// CGoliathXBurstProjectionBombsAttack's unit (tools/tu_map.py, unit 835): the attack's destructor
// (fn_7100586078): the listener at +0x70, the array of counted references at +0x38 (as UnkAttackRefArray), then CAttack's
// destructor (game_5821e4/CAttackDtor.h). Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "types.h"

extern void* g_unk_7101a4b150[];  // the attack's vtable

// UnkAttackRefArray's layout with elements counted in a union (game_s2/UnkWeakHandle.h's UnkRefObject): the
// original reloads the count after each release.
struct UnkBombRefs_ovb0113 {
    UnkRefObject** data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    UnkAttackAllocator* allocator;

    void Destroy() {
        if (data) {
            for (int i = 0; i < count; i++) {
                if (UnkRefObject* p = data[i]) {
                    if (--p->ref_count == 0)
                        p->vf_0x10();
                }
            }
            count = 0;
            allocator->Free(data);
            capacity = 0;
        }
    }
};

struct UnkBombsAttackDtor_ovb0113 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkBombRefs_ovb0113 bombs;  // 0x38
    u8 pad_0x60[0x10];
    UnkAttackListenerRaw listener;  // 0x70
};
static_assert(__builtin_offsetof(UnkBombsAttackDtor_ovb0113, listener) == 0x70, "layout");

void fn_7100586078(UnkBombsAttackDtor_ovb0113* self) {
    self->vtable = g_unk_7101a4b150 + 2;
    self->listener.Destroy();
    self->bombs.Destroy();
    fn_71003606c0(self);
}
