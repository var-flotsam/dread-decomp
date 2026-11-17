// CSamusMovementState::CParams's unit (tools/tu_map.py): a jump state's start: picks the collision the
// movement takes (its own, or the owner's named one when it cannot take one), plays the jump whoosh in
// the owner's mode 2, then hands the collision to the movement. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern "C" int rand();

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

__attribute__((visibility("hidden"))) extern CStrId g_unk_717273caf0;

// The sound description the jump plays, with its defaults.
struct UnkJumpSoundDesc_l9b1e14 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    CStrId name;          // 0x18
    CFilePathStrId path;  // 0x20
    f32 position[3];      // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkJumpSoundDesc_l9b1e14()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkJumpSoundDesc_l9b1e14();  // fn_710034e0b8
};
static_assert(__builtin_offsetof(UnkJumpSoundDesc_l9b1e14, field_0x6c) == 0x6c, "layout");

struct UnkJumpModeInfo_l9b1e14 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkJumpModeHolder_l9b1e14 {
    u8 pad_0x0[0x50];
    UnkJumpModeInfo_l9b1e14* info;  // 0x50
};

struct UnkJumpOwner_l9b1e14 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[8];
    UnkJumpModeHolder_l9b1e14* mode;  // 0x2b0
};

struct UnkJumpMovement_l9b1e14 {
    void** vtable;
    u8 pad_0x8[0x3a0];
    bool field_0x3a8;  // 0x3a8

    UnkCollisionObject* vf_0x2a8() {
        return reinterpret_cast<UnkCollisionObject* (*)(UnkJumpMovement_l9b1e14*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x2b0(UnkCollisionObject* collision) {
        reinterpret_cast<void (*)(UnkJumpMovement_l9b1e14*, UnkCollisionObject*)>(vtable[0x2b0 / 8])(this,
                                                                                                collision);
    }
};
static_assert(__builtin_offsetof(UnkJumpMovement_l9b1e14, field_0x3a8) == 0x3a8, "layout");

bool fn_71009ecd58(UnkJumpMovement_l9b1e14* movement);

struct UnkJumpState_l9b1e14 {
    u8 pad_0x0[0x30];
    UnkJumpOwner_l9b1e14* owner;        // 0x30
    UnkJumpMovement_l9b1e14* movement;  // 0x38
};

// Whether the owner's animation plays the converting-to-morph-ball track.
static inline bool IsConvertingToMorphBall(UnkJumpOwner_l9b1e14* owner) {
    return owner && owner->animation ? fn_710117b348(owner->animation, fn_7100cf2ae8()) : false;
}

void fn_71009b1e14(UnkJumpState_l9b1e14* self) {
    bool converting = IsConvertingToMorphBall(self->owner);
    UnkCollisionObject* collision;
    UnkJumpMovement_l9b1e14* movement = self->movement;
    if (converting || movement->field_0x3a8 || fn_71009ecd58(movement)) {
        collision = self->movement->vf_0x2a8();
        UnkJumpModeHolder_l9b1e14* mode = self->owner->mode;
        if (mode && mode->info && mode->info->mode == 2) {
            static CFilePathStrId s_whoosh1("actors/samus/samus_jumpwhoosh_01.wav", true);
            static CFilePathStrId s_whoosh2("actors/samus/samus_jumpwhoosh_02.wav", true);
            static UnkJumpSoundDesc_l9b1e14 s_desc;
            s_desc.kind = 0;
            // Both choices name the first whoosh in the original.
            s_desc.path = (rand() & 1) ? s_whoosh1 : s_whoosh1;
            s_desc.field_0x6d = true;
            s_desc.field_0x38 = 0.0f;
            s_desc.field_0x3c = 500.0f;
            s_desc.field_0x40 = 3000.0f;
            s_desc.field_0x44 = 1.3f;
            fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_desc, &g_unk_7101d470e0, false,
                          true, true);
        }
    } else {
        collision = fn_71008640a4(self->owner->collision, &g_unk_717273caf0);
    }
    self->movement->vf_0x2b0(collision);
}
