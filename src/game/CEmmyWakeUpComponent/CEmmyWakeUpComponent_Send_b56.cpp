// CEmmyWakeUpComponent's unit (tools/tu_map.py): fn_71007d723c calls fn_71007dc328, then, when the
// owner (+0x30) has its sub-object at +0x220 and it takes the hash of "walk" with mode 0 (sent as
// UnkSpitSend, src/game_s2/UnkSpitSend.h), moves the link at +0xc0 over to that sub-object: unhooks
// it from the target of the weak handle at +0xd8 and drops the handle, takes the sub-object's handle
// (fn_7100316018) and hooks the link to its target (fn_710117b3e8), as the attack track links in
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp do. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CCharClassEventPropComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

struct UnkEmmyWakeUpOwner_b56 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkEmmyWakeUp_b56 {
    u8 pad_0x0[0x30];
    UnkEmmyWakeUpOwner_b56* owner;
    u8 pad_0x38[0xc0 - 0x38];
    u8 link[0x18];
    UnkWeakHandle* handle;
};

static inline bool SendWalk_b56(void* target, u64 hash) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71007d723c(UnkEmmyWakeUp_b56* self) {
    fn_71007dc328(self);
    if (!self->owner)
        return;
    UnkWeakTarget* track = self->owner->field_0x220;
    if (!track)
        return;
    if (!SendWalk_b56(track, base::global::CName("walk").hash))
        return;
    void* link = self->link;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    asm("" : "+r"(self));
    fn_7100316018(track, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
