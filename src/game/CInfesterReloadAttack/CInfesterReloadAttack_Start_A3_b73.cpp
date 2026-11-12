// CInfesterReloadAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac),
// keeps the object the owner (+0x8) tracks (+0x220) at +0x50 and, when there is one, hooks the callback
// fn_71005c5cfc (link at +0x30, handle at +0x48) up to it (as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp); then hands the owner's navigation
// object (+0x298) a kind-5 parameter block (game_s2/UnkParams.h, fn_71006f4b0c with 1). Names and layouts
// are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_71005c5cfc();

struct UnkReloadTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

static inline UnkWeakTarget*& Track_A3_b73(char* owner) {
    return *reinterpret_cast<UnkWeakTarget**>(owner + 0x220);
}

struct UnkReloadAttack_A3_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    UnkReloadTrackLink_A3_b73 link;  // 0x30
    UnkWeakHandle* handle;           // 0x48
    UnkWeakTarget* field_0x50;
};

void fn_71005c54e4(UnkReloadAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x50 = Track_A3_b73(self->owner);
    if (Track_A3_b73(self->owner)) {
        UnkReloadTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_71005c5cfc;
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
    params.field_0x30 = 0;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    fn_71006f4b0c(nav, &params, 1);
}
