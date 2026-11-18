// CShakernautAIComponent unit (tools/tu_map.py unit 1047): the start (fn_71006adda8). After the base start it
// resets the two repeated actions to the tuning's interval, sizes both slot arrays to the tuning's count and
// clears their timers, takes the facing from the owner's matrix and copies the movement's range in.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/CShakernautAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU55.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

// One slot: a timer (below zero when idle) and a counted reference released with the slot.
struct UnkShakernautSlot_l6adda8 {
    f32 timer;
    u32 field_0x4;
    u64 field_0x8;
    u64 field_0x10;
    UnkRefObject* ref;

    void Release() {
        UnkRefObject* r = ref;
        if (r && --r->ref_count == 0)
            r->vf_0x10();
    }
};

using UnkShakernautSlots_l6adda8 = base::global::CRntVector<UnkShakernautSlot_l6adda8>;

// Grows the array's storage to hold n slots.
void fn_71006b2318(UnkShakernautSlots_l6adda8* slots, u32 n);

struct UnkShakernautTuning_l6adda8 {
    u8 pad_0x0[0xf0];
    u32 count;     // 0xf0
    f32 interval;  // 0xf4
};

struct UnkShakernautAIView_l6adda8 {
    u8 pad_0x0[0x146f0];
    UnkShakernautTuning_l6adda8* tuning;
};

// The movement's range, at +0x8e8 of the movement.
struct UnkShakernautRange_l6adda8 {
    u8 pad_0x0[0x8c];
    f32 min;
    f32 max;
};

struct UnkShakernautMovement_l6adda8 {
    u8 pad_0x0[0x8e8];
    UnkShakernautRange_l6adda8 range;
};

struct UnkShakernautOwner_l6adda8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    UnkReflObject* ai;  // 0x240
};

struct UnkShakernautAI_l6adda8 {
    u8 pad_0x0[0x30];
    UnkShakernautOwner_l6adda8* owner;  // 0x30
    u8 pad_0x38[0x14808 - 0x38];
    f32 timer[2];          // 0x14808
    f32 facing[2];         // 0x14810
    UnkShakernautSlots_l6adda8 slots_a;  // 0x14818
    UnkShakernautSlots_l6adda8 slots_b;  // 0x14840
    bool active[2];        // 0x14868
    u8 pad_0x1486a[2];
    u32 done[2];           // 0x1486c
    f32 elapsed[2];        // 0x14874
    f32 range_a[2];        // 0x1487c
    f32 range_b[2];        // 0x14884
    u8 pad_0x1488c[0x148a0 - 0x1488c];
    u32 field_0x148a0;
};
static_assert(__builtin_offsetof(UnkShakernautAI_l6adda8, slots_a) == 0x14818, "layout");
static_assert(__builtin_offsetof(UnkShakernautAI_l6adda8, field_0x148a0) == 0x148a0, "layout");

void fn_71006ad858(UnkShakernautAI_l6adda8* self);

// Sets the array to n slots: new ones are idle, dropped ones release their reference.
static inline void ResizeSlots_l6adda8(UnkShakernautSlots_l6adda8* slots, u32 n) {
    if (slots->count < n) {
        fn_71006b2318(slots, n);
        const UnkShakernautSlot_l6adda8 idle = {-1.0f, 0, 0, 0, nullptr};
        for (u64 i = slots->count; i < n; ++i)
            slots->data[i] = idle;
    } else if (slots->count == n) {
        return;
    } else {
        for (u64 i = n; i < slots->count; ++i)
            slots->data[i].Release();
    }
    slots->count = n;
}

void fn_71006adda8(UnkShakernautAI_l6adda8* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkReflObject* ai = self->owner->ai;
    const CClass* cls = ai->vf_0x0();
    fn_71006ad0a4();
    UnkShakernautAIView_l6adda8* comp =
        fn_710009d50c(cls, &g_unk_7172706520) ? reinterpret_cast<UnkShakernautAIView_l6adda8*>(ai) : nullptr;
    UnkShakernautTuning_l6adda8* tuning = comp->tuning;

    self->active[0] = false;
    self->active[1] = false;
    self->timer[1] = 0.0f;
    self->done[0] = 0;
    self->field_0x148a0 = 0;
    self->elapsed[0] = tuning->interval;
    self->done[1] = 0;
    self->elapsed[1] = tuning->interval;

    ResizeSlots_l6adda8(&self->slots_a, tuning->count);
    ResizeSlots_l6adda8(&self->slots_b, tuning->count);
    for (UnkShakernautSlot_l6adda8& slot : self->slots_a)
        slot.timer = -1.0f;
    for (UnkShakernautSlot_l6adda8& slot : self->slots_b)
        slot.timer = -1.0f;

    const UnkMatrix44& m = self->owner->xform.GetMatrix();
    UnkVec3f dir(m.m[2], m.m[6], m.m[10]);
    fn_710007b444(self->facing, reinterpret_cast<const UnkVec3*>(&dir));
    fn_71006ad858(self);

    UnkReflObject* movement = self->owner->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
        movement = nullptr;
    UnkShakernautRange_l6adda8* range =
        fn_71010cc3a4(movement) == 1 ? &reinterpret_cast<UnkShakernautMovement_l6adda8*>(movement)->range : nullptr;
    self->range_a[0] = range->min;
    self->range_a[1] = range->max;
    self->range_b[0] = range->min;
    self->range_b[1] = range->max;
}
