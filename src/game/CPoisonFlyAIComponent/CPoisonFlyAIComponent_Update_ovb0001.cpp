// CPoisonFlyAIComponent unit (tools/tu_map.py): fn_71006396dc picks one of two tuning values by whether the
// character class's word at +0x608 is zero; fn_71006392cc, an update step, after the base update records
// whether the owner's current state (+0x298) is of kind 2 with a non-negative value at +0xac, and when the
// manager g_unk_71726af468 (created on first use) has an attack offset entry for the owner, the target
// position as the target actor's position plus that offset, then goes on to fn_71006393d4;
// fn_710063970c, after fn_7101108124, faces the owner's movement (+0x218, slot 0x208) left or right
// toward its target and sets value 1 (fn_710111094c). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "game/CComponent.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The AI tuning constants (g_unk_71726c4890), as far as these units read them. Each pair of values is
// read as two differently nested fields, so the choice between them stays a choice of address.
struct UnkAITunablesValue_ovb0001 {
    f32 value;
};

struct UnkAITunables_ovb0001 {
    u8 pad_0x0[0x30];
    UnkAITunablesValue_ovb0001 field_0x30;
    f32 field_0x34;
    u8 pad_0x38[0x8];
    UnkAITunablesValue_ovb0001 field_0x40;
    f32 field_0x44;
    u8 pad_0x48[0x10];
    f32 field_0x58;
    u8 pad_0x5c[0x4];
    UnkAITunablesValue_ovb0001 field_0x60;
};
extern UnkAITunables_ovb0001 g_unk_71726c4890;

struct UnkPoisonFlyCharClass_ovb0001 {
    u8 pad_0x0[0x608];
    s32 field_0x608;
};

struct UnkPoisonFlyAI_ovb0001 {
    u8 pad_0x0[0x14758];
    UnkPoisonFlyCharClass_ovb0001* charClass;  // 0x14758
};

f32 fn_71006396dc(UnkPoisonFlyAI_ovb0001* self) {
    return self->charClass->field_0x608 == 0 ? g_unk_71726c4890.field_0x30.value : g_unk_71726c4890.field_0x44;
}

// As src/game/CAbilityEnergyWave/fn_71002ffea8.cpp declares it.
struct UnkParent;
// As src/game/CPoisonFlyAIComponent/CPoisonFlyAIComponent_OffsetTime_b67a4.cpp declares it.
struct UnkPoisonFlyAI_b67a4;
void fn_71006393d4(UnkPoisonFlyAI_b67a4* self, f32 dt);
// As src/game/UnkUnit958/UnkUnit958_b35.cpp declares it.
struct UnkUnit958List;
struct UnkUnit958Entry;
UnkUnit958Entry* fn_710063b770(UnkUnit958List* self, void* key);

struct UnkPoisonFlyOffsetEntry_ovb0001 {
    u8 pad_0x0[0x14];
    f32 x, y, z;  // 0x14
};

struct UnkPoisonFlyState_ovb0001 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
    u8 pad_0x14[0x98];
    s32 field_0xac;
};

struct UnkPoisonFlyStateHolder_ovb0001 {
    u8 pad_0x0[0x10];
    UnkPoisonFlyState_ovb0001* state;  // 0x10
};

struct UnkPoisonFlyStateSlot_ovb0001 {
    UnkPoisonFlyStateHolder_ovb0001* holder;
};

struct UnkPoisonFlyStates_ovb0001 {
    u8 pad_0x0[0xb0];
    UnkPoisonFlyStateSlot_ovb0001* slot;  // 0xb0
};

struct UnkPoisonFlyOwner_ovb0001 {
    u8 pad_0x0[0x298];
    UnkPoisonFlyStates_ovb0001* states;  // 0x298
};

struct UnkPoisonFlyTarget_ovb0001 {
    bool active;
    u8 pad_0x1[0x3];
    f32 x, y, z;  // 0x4
};

struct UnkPoisonFlyAI1_ovb0001 {
    u8 pad_0x0[0x30];
    UnkPoisonFlyOwner_ovb0001* owner;  // 0x30
    u8 pad_0x38[0x14760 - 0x38];
    UnkPoisonFlyTarget_ovb0001 target;  // 0x14760
};

void fn_71006392cc(UnkPoisonFlyAI1_ovb0001* self, f32 dt) {
    UnkPoisonFlyTarget_ovb0001* target = &self->target;
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPoisonFlyState_ovb0001* state = nullptr;
    if (UnkPoisonFlyStates_ovb0001* states = self->owner->states)
        if (UnkPoisonFlyStateHolder_ovb0001* holder = states->slot->holder)
            state = holder->state;
    if (state && state->kind == 2)
        target->active = state->field_0xac >= 0;
    else
        target->active = false;
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    UnkPoisonFlyOffsetEntry_ovb0001* entry = reinterpret_cast<UnkPoisonFlyOffsetEntry_ovb0001*>(fn_710063b770(
        reinterpret_cast<UnkUnit958List*>(g_unk_71726af468->pad_0x0 + 0x80), self->owner));
    if (entry) {
        UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        f32 x = actor->field_0x44 + entry->x;
        f32 y = actor->field_0x54 + entry->y;
        f32 z = actor->field_0x64 + entry->z;
        target->x = x;
        target->y = y;
        target->z = z;
    }
    fn_71006393d4(reinterpret_cast<UnkPoisonFlyAI_b67a4*>(self), dt);
}

// As src/game/CInfesterAIComponent/CInfesterAIComponent_Send_b57.cpp declares it.
struct UnkQuetzoaAIComponent;
// As src/game/CBigFistAttack/CBigFistAttack_Start_A2_b72.cpp declares it.
void fn_710111094c(UnkReflObject* ai, u32 value);

struct UnkPoisonFlyVec_ovb0001 {
    f32 x, y, z;
};


struct UnkPoisonFlyAI2_ovb0001 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

void fn_710063970c(UnkPoisonFlyAI2_ovb0001* self, void* arg) {
    fn_7101108124(reinterpret_cast<UnkQuetzoaAIComponent*>(self), arg);
    UnkActor* owner = self->owner;
    UnkPoisonFlyVec_ovb0001 from = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    UnkActor* target = fn_7100d7c990(owner);
    UnkPoisonFlyVec_ovb0001 to = {target->field_0x44, target->field_0x54, target->field_0x64};
    UnkPoisonFlyVec_ovb0001 diff = {to.x - from.x, to.y - from.y, to.z - from.z};
    f32 inv = 1.0f / (sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z) + 0.0001f);
    UnkPoisonFlyVec_ovb0001 n = {diff.x * inv, diff.y * inv, diff.z * inv};
    UnkPoisonFlyVec_ovb0001 dir;
    dir.x = 1.0f;
    dir.y = 0.0f;
    dir.z = 0.0f;
    if (n.x * dir.x + n.y * dir.y + n.z * dir.z < 0.0f) {
        dir.x = -dir.x;
        dir.y = -dir.y;
        dir.z = -dir.z;
    }
    if (void* movement = self->owner->field_0x218)
        (*reinterpret_cast<void (***)(void*, UnkPoisonFlyVec_ovb0001*)>(movement))[0x208 / 8](movement, &dir);
    fn_710111094c(reinterpret_cast<UnkReflObject*>(self), 1);
}
