// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): releases the target of each of the five
// aim slots, takes every lock-on mark off the render manager and empties the mark list.
// Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMultiLockon_ovb0092;
struct UnkStruct_71002334fc;
void fn_710031965c(UnkMultiLockon_ovb0092* self, UnkStruct_71002334fc* target);

// A mark on screen: its fade time and the mark object (drawn through its item at +8).
struct UnkLockonEntry_l31d33c {
    f32 time;
    u8* mark;
};

struct UnkMultiLockon_l31d33c {
    u8 pad_0x0[0x3a0];
    UnkWeakHandle* slots[5];  // 0x3a0
    u8 pad_0x3c8[0x338];
    UnkLockonEntry_l31d33c* marks;  // 0x700
    u32 mark_count;                 // 0x708
    u8 pad_0x70c[0x1c];
    f32 field_0x728;
};
static_assert(__builtin_offsetof(UnkMultiLockon_l31d33c, field_0x728) == 0x728, "UnkMultiLockon_l31d33c layout");

// Holds a counted reference to a slot's handle for the length of the release.
struct UnkSlotRef_l31d33c {
    UnkWeakHandle* p;
    UnkSlotRef_l31d33c(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkSlotRef_l31d33c() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

void CAbilityMultiLockon_ReleaseTargetsAndClearFadingObjects(UnkMultiLockon_l31d33c* self) {
    for (u32 i = 0; i < 5; ++i) {
        UnkSlotRef_l31d33c ref(self->slots[i]);
        UnkWeakTarget* target = ref.p ? ref.p->target : nullptr;
        fn_710031965c(reinterpret_cast<UnkMultiLockon_ovb0092*>(self),
                      reinterpret_cast<UnkStruct_71002334fc*>(target));
    }
    UnkLockonEntry_l31d33c* e = self->marks;
    for (UnkLockonEntry_l31d33c* end = e + self->mark_count; e != end; ++e) {
        if (e->mark)
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(e->mark + 8), 0);
    }
    self->mark_count = 0;
    self->field_0x728 = -1.0f;
}
