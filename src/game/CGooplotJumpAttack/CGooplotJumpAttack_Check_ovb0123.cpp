// CGooplotJumpAttack's unit (tools/tu_map.py): the attack's start check (fn_71005944e8) and update
// (fn_71005951fc). The check needs the owner (+8) grounded (fn_7100d7bea0), its AI component (+0x240)
// flag at +0x121e and a landing point (fn_710059462c); a Gooplot with jumps left (+0x147f8) whose
// character class (+0x1610) is in mode 1 then also needs the Gooshocker check (fn_710059edf8) to fail.
// The update waits in state 0 until the lazily created manager (+0x138, fn_710059d7e4) lets the owner
// jump and the jump has lasted longer than +0x7c without a target, then goes to state 1; in state 1 a
// set flag (+0x60) moves it to state 2 (fn_7100594d5c). The start (fn_7100594c08) records a jump time
// of 0 for the owner (fn_7100595e28), takes two tunable words, finds the landing point, stores the
// owner's position in the attack and its AI component (+0x1489c, with the owner's x angle), lets a
// Gooshocker AI know (fn_710059f058) and enters state 0. The copy assignment (fn_7100596304) copies the
// CAttack part and the plain fields as blocks, shares the listener's (+0x38) counted reference to its
// owner's weak handle unless the attack is assigned to itself, and copies the six words at +0x64 one
// by one. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkSpitAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_7100d7bea0(UnkActor*, bool);

struct UnkGooplotJump_ovb0123;
bool fn_710059462c(UnkGooplotJump_ovb0123* self, base::math::CVector3D* point);
void fn_710059531c(UnkGooplotJump_ovb0123* self, u32 arg);
void fn_7100594d5c(UnkGooplotJump_ovb0123* self, u32 state);
bool fn_710059d7e4(void* member, UnkActor* owner);
bool fn_710059edf8(UnkReflObject* ai);

struct UnkGooplotClass_ovb0123 {
    u8 pad_0x0[0x6f0];
    s32 mode;  // 0x6f0
};

struct UnkGooplotJumpAI_ovb0123 {
    u8 pad_0x0[0x121e];
    bool field_0x121e;
    u8 pad_0x121f[0x1610 - 0x121f];
    UnkReflObject* char_class;  // 0x1610
    u8 pad_0x1618[0x147f8 - 0x1618];
    s32 jumps;  // 0x147f8
};

struct UnkGooplotJumpTarget_ovb0123 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkGooplotJumpTargets_ovb0123 {
    u8 pad_0x0[0x680];
    UnkGooplotJumpTarget_ovb0123* field_0x680;
};

struct UnkGooplotJump_ovb0123 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    s32 state;  // 0x30
    u8 pad_0x34[0x24];
    f32 time;  // 0x58
    u8 pad_0x5c[4];
    bool field_0x60;
    u8 pad_0x61[0x1b];
    f32 duration;  // 0x7c
};

static inline UnkGooplotJumpAI_ovb0123* Ai(UnkActor* owner) {
    return static_cast<UnkGooplotJumpAI_ovb0123*>(owner->field_0x240);
}

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

void fn_71005951fc(UnkGooplotJump_ovb0123* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    switch (self->state) {
    case 0: {
        if (Ai(self->owner)->field_0x121e)
            fn_710059531c(self, 0);
        if (!fn_710059d7e4(Manager()->pad_0x0 + 0x138, self->owner))
            return;
        self->time += dt;
        UnkActor* target = fn_7100d7c990(self->owner);
        auto* targets = static_cast<UnkGooplotJumpTargets_ovb0123*>(target->field_0x2b0);
        if (targets && targets->field_0x680 && targets->field_0x680->field_0x10)
            return;
        if (self->time > self->duration)
            fn_7100594d5c(self, 1);
        return;
    }
    case 1:
        if (self->field_0x60) {
            self->field_0x60 = false;
            fn_7100594d5c(self, 2);
        }
        return;
    }
}

bool fn_71005944e8(UnkGooplotJump_ovb0123* self, void* arg, u32 mode) {
    if (!fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode))
        return false;
    base::math::CVector3D point = g_unk_710164779c;
    u32 ok = false;
    if (fn_7100d7bea0(self->owner, false) && Ai(self->owner)->field_0x121e && fn_710059462c(self, &point)) {
        UnkGooplotJumpAI_ovb0123* ai = Ai(self->owner);
        if (!ai) {
            ok = true;
        } else {
            ok = ai->jumps > 0;
            UnkReflObject* cls = ai->char_class;
            auto* gooplot = reinterpret_cast<UnkGooplotClass_ovb0123*>(
                fn_710009d50c(cls->vf_0x0(), fn_7100598ca0()) ? cls : nullptr);
            if (gooplot->mode == 1) {
                UnkReflObject* other = static_cast<UnkReflObject*>(self->owner->field_0x240);
                if (other && fn_710009d50c(other->vf_0x0(), fn_710059e014()))
                    ok = !fn_710059edf8(other);
            }
        }
    }
    return ok != 0;
}

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
extern UnkTunable g_unk_71726e72b8;

struct UnkGooplotKeys_ovb0123 {
    u8 pad_0x0[0x30];
    u64 jump_time;  // 0x30
};
extern UnkGooplotKeys_ovb0123* g_unk_71726e7000 __attribute__((visibility("hidden")));

struct UnkGooplotRecords_ovb0123;
void fn_7100595e28(UnkGooplotRecords_ovb0123* self, const u64* key, f32 value);
void fn_710059f058(UnkReflObject* ai);

struct UnkGooplotTunable_ovb0123 {
    u8 pad_0x0[0x34];
    u32 field_0x34;
    u32 field_0x38;
};

struct UnkGooplotStartAI_ovb0123 {
    u8 pad_0x0[0x1489c];
    UnkVec3f position;  // 0x1489c
    f32 angle_x;        // 0x148a8
};

struct UnkGooplotStartOwner_ovb0123 {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x18c];
    UnkGooplotRecords_ovb0123* records;  // 0x220
};

struct UnkGooplotStart_ovb0123 {
    void** vtable;
    UnkGooplotStartOwner_ovb0123* owner;  // 0x8
    u8 pad_0x10[0x50];
    bool field_0x60;
    u8 pad_0x61[3];
    base::math::CVector3D point;  // 0x64
    UnkVec3f position;            // 0x70
    u32 field_0x7c;
    u32 field_0x80;
};

void fn_7100594c08(UnkGooplotStart_ovb0123* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    {
        auto* records = reinterpret_cast<UnkGooplotRecords_ovb0123*>(reinterpret_cast<u8*>(self->owner->records) + 0x80);
        u64 key = g_unk_71726e7000->jump_time;
        fn_7100595e28(records, &key, 0.0f);
    }
    self->field_0x60 = false;
    auto* tunable = reinterpret_cast<UnkGooplotTunable_ovb0123*>(&g_unk_71726e72b8);
    self->field_0x7c = tunable->field_0x34;
    self->field_0x80 = tunable->field_0x38;
    fn_710059462c(reinterpret_cast<UnkGooplotJump_ovb0123*>(self), &self->point);
    UnkActor* owner = reinterpret_cast<UnkActor*>(self->owner);
    self->position = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    if (auto* ai = static_cast<UnkGooplotStartAI_ovb0123*>(owner->field_0x240)) {
        ai->position = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        f32 angle_x = self->owner->transform.GetAngle().x;
        // An untyped store: the original reloads the owner after it.
        __builtin_memcpy(&ai->angle_x, &angle_x, sizeof(angle_x));
        UnkReflObject* other = static_cast<UnkReflObject*>(reinterpret_cast<UnkActor*>(self->owner)->field_0x240);
        if (other && fn_710009d50c(other->vf_0x0(), fn_710059e014()))
            fn_710059f058(other);
    }
    fn_7100594d5c(reinterpret_cast<UnkGooplotJump_ovb0123*>(self), 0);
}

struct UnkGooplotJumpCopy_ovb0123 {
    void** vtable;
    u8 base[0x20];  // 0x8, the CAttack part
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[4];
    void** listener_vtable;  // 0x38
    u8 listener[0x10];       // 0x40
    UnkWeakHandle* owner;    // 0x50
    u64 field_0x58;
    bool field_0x60;
    u8 pad_0x61[3];
    u32 words[6];  // 0x64
    u32 field_0x7c;
    u32 field_0x80;
};
static_assert(__builtin_offsetof(UnkGooplotJumpCopy_ovb0123, field_0x80) == 0x80, "layout");

UnkGooplotJumpCopy_ovb0123* fn_7100596304(UnkGooplotJumpCopy_ovb0123* self, const UnkGooplotJumpCopy_ovb0123* other) {
    __builtin_memcpy(self->base, other->base, sizeof(self->base));
    __builtin_memcpy(&self->field_0x28, &other->field_0x28, 0xc);
    __builtin_memcpy(self->listener, other->listener, sizeof(self->listener));
    if (self != other)
        CopyWeakRef(&self->owner, other->owner);
    __builtin_memcpy(&self->field_0x58, &other->field_0x58, 9);
    for (u32 i = 0; i < 6; ++i)
        self->words[i] = other->words[i];
    __builtin_memcpy(&self->field_0x7c, &other->field_0x7c, 8);
    return self;
}
