// CCharacterLifeComponent unit (tools/tu_map.py unit 2713): takes a fraction of the life (fn_71011603a4),
// notifies the owner's (+0x30) objects at +0x200 (vf_0x1e8 with 1) and +0x1d8 (fn_7100871d7c), sends the owner's
// track (+0x220) its hash with mode 1 when the life changed and `flag` is set, then, when the handle at
// +0x240 still has a target, places the owner at the target's position and drops the handle. Returns
// whether the life changed. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCharacterLifeComponent.h"
#include "game/CInputComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkInputComponentState;

struct UnkLifeHashHolder_l158d78 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkLifeTrack_l158d78 {
    u8 pad_0x0[0x2b8];
    UnkLifeHashHolder_l158d78* field_0x2b8;
};

struct UnkLifeOwner_l158d78 {
    void** vtable;
    u8 pad_0x8[0x1d0];
    UnkInputComponentState* input;  // 0x1d8
    u8 pad_0x1e0[0x20];
    void* field_0x200;
    u8 pad_0x208[0x18];
    UnkLifeTrack_l158d78* track;  // 0x220

    // vtable slot 0xd0: places the actor at `pos`.
    void vf_0xd0(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkLifeOwner_l158d78*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};

struct UnkCharacterLifeComponent {
    u8 pad_0x0[0x30];
    UnkLifeOwner_l158d78* owner;  // 0x30
    u8 pad_0x38[0x208];
    UnkWeakHandle* handle;  // 0x240
};

bool fn_7101158d78(UnkCharacterLifeComponent* self, f32 fraction, bool flag) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    bool changed = fn_71011603a4(reinterpret_cast<UnkLifeValues_b49*>(self), fraction);
    if (void* p = self->owner->field_0x200)
        reinterpret_cast<void (*)(void*, u32)>((*reinterpret_cast<void***>(p))[0x1e8 / 8])(p, 1);
    if (UnkInputComponentState* input = self->owner->input)
        fn_7100871d7c(input);
    if (changed && flag) {
        if (UnkLifeTrack_l158d78* track = self->owner->track) {
            u64 hash = track->field_0x2b8->hash;
            mode.id = 1;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(track, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkActor* target = reinterpret_cast<UnkActor*>(handle->target)) {
            UnkLifeOwner_l158d78* owner = self->owner;
            UnkVec3f pos(target->field_0x44, target->field_0x54, target->field_0x64);
            owner->vf_0xd0(&pos, true);
            ResetRef(&self->handle);
        }
    }
    return changed;
}
