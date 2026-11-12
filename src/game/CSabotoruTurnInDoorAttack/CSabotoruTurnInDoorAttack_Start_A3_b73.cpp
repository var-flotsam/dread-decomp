// CSabotoruTurnInDoorAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac)
// clears +0x30, keeps the owner's (+0x8) character class (+0x220) at +0x58, stops its layers 6 and 0xc and
// plays layer 0xb, calls fn_710067342c with 1, hooks the callback fn_71006741bc (link at +0x38, handle at
// +0x50) up to the class, as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, and hands the
// owner's navigation object (+0x298) a kind-5 parameter block (game_s2/UnkParams.h, fn_71006f4b0c with 1).
// Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CSabotoruTurnInDoorAttack.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_71006741bc();
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, bool, bool, f32);
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);

struct UnkSabotoruTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

static inline UnkWeakTarget*& Track_A3_b73(char* owner) {
    return *reinterpret_cast<UnkWeakTarget**>(owner + 0x220);
}

struct UnkSabotoruAttack_A3_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    UnkSabotoruTrackLink_A3_b73 link;  // 0x38
    UnkWeakHandle* handle;             // 0x50
    UnkActorTarget* anim;              // 0x58
};

void fn_71006732b4(UnkSabotoruAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 0;
    self->anim = reinterpret_cast<UnkActorTarget*>(Track_A3_b73(self->owner));
    fn_7101178e08(self->anim, 6, 0, 0, -1.0f);
    fn_7101178e08(self->anim, 0xc, 0, 0, -1.0f);
    fn_710117662c(self->anim, 0xb, 0, false, false, -1.0f);
    fn_710067342c(self, true);
    if (Track_A3_b73(self->owner)) {
        UnkSabotoruTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_71006741bc;
        UnkWeakTarget* track = Track_A3_b73(self->owner);
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
    }
    void* nav = *reinterpret_cast<void**>(self->owner + 0x298);
    UnkParams params;
    params.field_0x1c = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x34 = 5;
    params.pad_0x38 = 1;
    params.field_0x39 = 1;
    params.pad_0x3a = 0;
    params.field_0x3b = 1;
    params.field_0x3d = 0;
    fn_71006f4b0c(nav, &params, 1);
}
