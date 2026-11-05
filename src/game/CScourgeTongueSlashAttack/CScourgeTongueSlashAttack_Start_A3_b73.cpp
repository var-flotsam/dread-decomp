// CScourgeTongueSlashAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac)
// sets the pair at +0x30 to (0, 1) and +0x60 to -1, hooks the callback fn_710069cd7c (link at +0x40, handle
// at +0x58) up to the owner's (+0x8) character class (+0x220), as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp; hands the owner's navigation object
// (+0x298) a kind-5 parameter block (game_s2/UnkParams.h, fn_71006f4b0c with 1); when the owner's AI
// component (+0x240) is a CScourgeAIComponent, copies its pair at +0x14740 to +0x64 and calls fn_710069f43c
// on it; finally hands the owner to the registry at +0x5e0 of the manager g_unk_71726af468 (created on
// first use). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_710069cd7c();
void fn_710069f43c(UnkReflObject* ai);

struct UnkScourgeOwner_A3_b73;
void fn_71006a23c4(void* registry, UnkScourgeOwner_A3_b73* owner);

struct UnkScourgeOwner_A3_b73 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
    u8 pad_0x248[0x298 - 0x248];
    void* field_0x298;  // the navigation object
};

struct UnkScourgeTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkScourgeAttack_A3_b73 {
    u8 pad_0x0[0x8];
    UnkScourgeOwner_A3_b73* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
    u8 pad_0x38[0x8];
    UnkScourgeTrackLink_A3_b73 link;  // 0x40
    UnkWeakHandle* handle;            // 0x58
    f32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
};

void fn_710069b178(UnkScourgeAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 0;
    self->field_0x34 = 1;
    self->field_0x60 = -1.0f;
    if (*reinterpret_cast<UnkWeakTarget**>(reinterpret_cast<char*>(self->owner) + 0x220)) {
        UnkScourgeTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_710069cd7c;
        UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(reinterpret_cast<char*>(self->owner) + 0x220);
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
    if (void* nav = self->owner->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(nav, &params, 1);
    }
    if (UnkReflObject* ai = self->owner->field_0x240) {
        if (fn_710009d50c(ai->vf_0x0(), fn_710069ef44())) {
            u32* pair = reinterpret_cast<u32*>(reinterpret_cast<char*>(ai) + 0x14740);
            self->field_0x64 = pair[0];
            self->field_0x68 = pair[1];
            fn_710069f43c(ai);
        }
    }
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_71006a23c4(reinterpret_cast<char*>(manager) + 0x5e0, self->owner);
}
