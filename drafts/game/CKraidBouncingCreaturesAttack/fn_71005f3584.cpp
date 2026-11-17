// The CKraidBouncingCreaturesAttack unit: on an event of kind 1, while the current list (+0x38) of the pattern
// set (+0x30) has slots left past +0x3c and the owner's Kraid AI does not stop it (fn_71005fd848), sends the
// owner's animation (+0x220) the next launch with mode 2 (the entry's time kept at +0x40); otherwise sends the
// set's end id with mode 0 and returns false. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBouncingCreaturesLaunchPattern.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

bool fn_71005fd848(UnkReflObject* kraid);

struct UnkKraidBouncingList_l5f3584 {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkKraidBouncingEntry_l5f3584 {
    u8 pad_0x0[0x14];
    f32 time;
};

struct UnkKraidBouncingOwner_l5f3584 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkReflObject* ai;  // 0x240
};

struct UnkKraidBouncingSet_l5f3584 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CStrId* vf_0x18(UnkKraidBouncingOwner_l5f3584* owner);
    u8 pad_0x8[0xf0 - 0x8];
    CStrId end_id;  // 0xf0
    UnkKraidBouncingList_l5f3584* lists;  // 0xf8
};

struct UnkKraidBouncingAttack_l5f3584 {
    u8 pad_0x0[0x8];
    UnkKraidBouncingOwner_l5f3584* owner;  // 0x8
    u8 pad_0x10[0x30 - 0x10];
    UnkKraidBouncingSet_l5f3584* set;  // 0x30
    u32 index;  // 0x38
    u32 slot;  // 0x3c
    f32 time;  // 0x40
};

struct UnkKraidBouncingEvent_l5f3584 {
    u32 kind;
};

// The list is typed as CBouncingCreaturesLaunchPattern.h declares it.
struct UnkKraidBouncingList_b57a4;

static inline void Send(void* animation, u64 hash, u32 m, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = m;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

bool fn_71005f3584(UnkKraidBouncingAttack_l5f3584* self, const UnkKraidBouncingEvent_l5f3584* event) {
    // Declared past a goto (no lifetime markers), so each request's mode and mask keep a slot of their own.
    goto start;
    base::reflection::UnkReflTypeIdArg launch_mode;
    base::reflection::UnkReflTypeIdArg launch_mask;
    base::reflection::UnkReflTypeIdArg next_mode;
    base::reflection::UnkReflTypeIdArg next_mask;
    base::reflection::UnkReflTypeIdArg end_mode;
    base::reflection::UnkReflTypeIdArg end_mask;
start:
    if (event->kind != 1)
        return true;
    void* animation = self->owner->animation;
    if (self->slot != self->set->lists[self->index].count) {
        UnkReflObject* ai = self->owner->ai;
        UnkReflObject* kraid = ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? ai : nullptr;
        if (!fn_71005fd848(kraid)) {
            UnkKraidBouncingList_l5f3584* list = &self->set->lists[self->index];
            auto* entry = static_cast<UnkKraidBouncingEntry_l5f3584*>(
                *fn_71005e2c08(reinterpret_cast<UnkKraidBouncingList_b57a4*>(list), self->slot));
            if (entry && entry->time != 0.0f) {
                Send(animation, 0xa5133ff8f4e5d6c8ull, 2, &launch_mode, &launch_mask);
                self->time = entry->time;
            } else {
                u64 hash = self->set->vf_0x18(self->owner)->data->Key();
                Send(animation, hash, 2, &next_mode, &next_mask);
                self->time = 0.0f;
            }
            return true;
        }
    }
    u64 hash = self->set->end_id.data->Key();
    Send(animation, hash, 0, &end_mode, &end_mask);
    return false;
}
