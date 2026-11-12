// behaviortree::CScorpiusRegereratingMaskAction (tools/tu_map.py): starting the action. After the base start
// (fn_7100394bd0), hooks the callback fn_710068db84 (link at +0x38, handle at +0x50) up to the owner's (+0x18)
// character class (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp; sets +0x2c
// to whether the class's track 0xf4061ff25a2163d6 (fn_7101177d2c) has the kind 0x9f3eb4d219b29df9, clears +0x30,
// puts 1.0 into the class's bag (+0x80) under the key g_unk_7172703bd8 and sets +0xac of the tunable
// g_unk_7172790a08 to 1. Names and layouts are placeholders.
#include "../../../game_s7/UnkPropBagS7.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
void fn_710068db84();
struct UnkAnimationComponent;

struct UnkGlobal2;
extern char g_unk_7172703bd8[] __attribute__((visibility("hidden")));  // the bag key

struct UnkMaskTunable_A3_b73 {
    u8 pad_0x0[0xac];
    f32 field_0xac;
};

struct UnkMaskTrackKind_A3_b73 {
    u8 pad_0x0[0x30];
    u64 kind;
};

struct UnkMaskTrack_A3_b73 {
    u8 pad_0x0[0x38];
    UnkMaskTrackKind_A3_b73* kind;
};

struct UnkMaskOwner_A3_b73 {
    u8 pad_0x0[0x220];
    char* field_0x220;
};

struct UnkMaskTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkMaskStart_A3_b73 {
    u8 pad_0x0[0x18];
    UnkMaskOwner_A3_b73* owner;
    u8 pad_0x20[0xc];
    u32 field_0x2c;
    u64 field_0x30;
    UnkMaskTrackLink_A3_b73 link;  // 0x38
    UnkWeakHandle* handle;         // 0x50
};

void fn_710068ce30(UnkMaskStart_A3_b73* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkWeakTarget* track = reinterpret_cast<UnkWeakTarget*>(self->owner->field_0x220);
    UnkMaskTrackLink_A3_b73* link = &self->link;
    link->context = self;
    link->callback = fn_710068db84;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    UnkMaskTrack_A3_b73* t = static_cast<UnkMaskTrack_A3_b73*>(
        fn_7101177d2c(reinterpret_cast<UnkAnimationComponent*>(track), 0xf4061ff25a2163d6));
    bool kind = t->kind->kind == 0x9f3eb4d219b29df9;
    self->field_0x30 = 0;
    self->field_0x2c = kind;
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(self->owner->field_0x220 + 0x80),
                                *reinterpret_cast<const base::global::CStrId*>(g_unk_7172703bd8), 1.0f);
    reinterpret_cast<UnkMaskTunable_A3_b73*>(&g_unk_7172790a08)->field_0xac = 1.0f;
}
