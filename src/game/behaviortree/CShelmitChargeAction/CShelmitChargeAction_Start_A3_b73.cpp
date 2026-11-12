// behaviortree::CShelmitChargeAction (tools/tu_map.py): starting the action. After the base start
// (fn_7100394bd0), hands the owner's (+0x18) navigation object (+0x298) a kind-5 parameter block
// (game_s2/UnkParams.h, as fn_71006b6dc8 in CShelmitPlasmaRayAttack_Nav_b65a4.cpp, with 0), sets +0x50 to 1 and
// +0x54 to -1, then hooks the callback fn_71006b7db4 (link at +0x30, handle at +0x48) up to the owner's
// character class (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp. Names and
// layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
void fn_7100394bd0(UnkSelf*);
void fn_71006b7db4();

struct UnkShelmitChargeOwner_A3_b73 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x298 - 0x228];
    void* field_0x298;  // the navigation object
};

struct UnkShelmitChargeTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkShelmitCharge_A3_b73 {
    u8 pad_0x0[0x18];
    UnkShelmitChargeOwner_A3_b73* owner;
    u8 pad_0x20[0x10];
    UnkShelmitChargeTrackLink_A3_b73 link;  // 0x30
    UnkWeakHandle* handle;                  // 0x48
    u32 field_0x50;
    f32 field_0x54;
};

void fn_71006b7350(UnkShelmitCharge_A3_b73* self) {
    fn_7100394bd0(reinterpret_cast<UnkSelf*>(self));
    UnkShelmitChargeOwner_A3_b73* owner = self->owner;
    if (owner) {
        if (void* nav = owner->field_0x298) {
            UnkParams params;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x34 = 5;
            *reinterpret_cast<u16*>(&params.pad_0x3c) = 0x100;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            fn_71006f4b0c(nav, &params, 0);
        }
    }
    self->field_0x50 = 1;
    self->field_0x54 = -1.0f;
    UnkWeakTarget* track = self->owner ? self->owner->field_0x220 : nullptr;
    if (track) {
        UnkShelmitChargeTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_71006b7db4;
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
}
