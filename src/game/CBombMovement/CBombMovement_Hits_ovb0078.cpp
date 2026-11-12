// CBombMovement's unit (tools/tu_map.py): the bomb's position, hit handling and explosion requests.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSceneComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

// Whether the hit was already handled (bombs that touch the player or something they ignore).
bool fn_7100ac79d8(void* self, void* actor, void* info);

struct UnkBombDirSource_ovb0078 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBombDirHolder_ovb0078 {
    u8 pad_0x0[0x128];
    UnkBombDirSource_ovb0078* field_0x128;
    u8 pad_0x130[0x18];
    UnkVec3f dir;  // 0x148
};

struct UnkBombActor_ovb0078 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    UnkBombDirHolder_ovb0078* field_0x1f0;
    u8 pad_0x1f8[0x37c - 0x1f8];
    f32 radius;  // 0x37c
};

struct UnkBombMovement_ovb0078 {
    u8 pad_0x0[0x30];
    UnkBombActor_ovb0078* owner;
};

void fn_7100aabf4c(UnkBombMovement_ovb0078* self, UnkBombActor_ovb0078* actor, UnkVec3f* pos,
                   UnkVec3f* offset) {
    *pos = UnkVec3f(actor->x, actor->y, actor->z);
    *offset = UnkVec3f(0.0f, 0.0f, 0.0f);
    f32* p = &pos->x;
    UnkBombDirHolder_ovb0078* holder = actor->field_0x1f0;
    if (holder->field_0x128 && holder->field_0x128->field_0x10) {
        f32 dx = holder->dir.x;
        f32 dy = holder->dir.y;
        f32 dz = holder->dir.z;
        f32 scale = actor->radius * 0.33f;
        dx = dx * scale;
        dy = dy * scale;
        dz = dz * scale;
        p[0] = p[0] + dx;
        p[1] = p[1] + dy;
        p[2] = p[2] + dz;
    } else {
        p[1] = p[1] + actor->radius * -0.33f;
    }
    p[2] = p[2] + (actor->radius + self->owner->radius);
}

struct UnkListedItem;
base::UnkVector<UnkListedItem*>* fn_7100552d7c();
void fn_7100558ed8(void* entity, void* movement);
extern u64 g_unk_710164b688;

struct UnkBombPos_ovb0078 {
    f32 x, y, z;
};

struct UnkBombListedItem_ovb0078 {
    u8 pad_0x0[0x10];
    void* entity;
};

struct UnkBombMovementState_ovb0078 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x488 - 0x38];
    bool field_0x488;
    u8 pad_0x489[0x4b0 - 0x489];
    UnkRefObject* field_0x4b0;
};

void fn_7100aaa6d0(UnkBombMovementState_ovb0078* self) {
    self->field_0x488 = true;
    ResetRef(&self->field_0x4b0);
    if (void* shape = self->owner->field_0x228)
        fn_71009f19b0(shape, false);
    fn_7100d7c828(self->owner, &g_unk_710164b688, false);
    base::UnkVector<UnkListedItem*>* items = fn_7100552d7c();
    UnkListedItem** it = items->data;
    UnkListedItem** end = it + items->count;
    for (; it != end; ++it) {
        auto* item = reinterpret_cast<UnkBombListedItem_ovb0078*>(*it);
        if (item && item->entity)
            fn_7100558ed8(item->entity, self);
    }
    UnkGameManager* manager = g_unk_7101d47310;
    UnkActor* owner = self->owner;
    UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    fn_710056a010(reinterpret_cast<char*>(manager) + 0x20a8, &pos);
}

struct UnkPlayer;

struct UnkBombHitActor_ovb0078 {
    u8 pad_0x0[0x1f0];
    base::reflection::UnkReflObject* field_0x1f0;  // the movement
};

struct UnkBombHitMovement_ovb0078;
void fn_710094b89c(base::reflection::UnkReflObject* movement, const UnkVec3f* pos);

struct UnkBombHitMovementVtbl_ovb0078 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkBombHitMovement_ovb0078* self, UnkBombHitActor_ovb0078* actor, void* info);
    void* slots_0x2b0[2];
    void (*vf_0x2c0)(UnkBombHitMovement_ovb0078* self, UnkBombHitActor_ovb0078* actor, void* a2, void* a3,
                     void* info);
};

struct UnkBombHitMovement_ovb0078 {
    UnkBombHitMovementVtbl_ovb0078* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
};

void fn_7100aabc9c(UnkBombHitMovement_ovb0078* self, UnkBombHitActor_ovb0078* actor, void* a2, void* a3,
                   void* info) {
    if (reinterpret_cast<UnkBombHitActor_ovb0078*>(fn_7100dec788(g_unk_7101d47310)) != actor) {
        self->vtbl->vf_0x2c0(self, actor, a2, a3, info);
        return;
    }
    if (fn_7100ac79d8(self, actor, info))
        return;
    self->vtbl->vf_0x2a8(self, actor, info);
    base::reflection::UnkReflObject* movement = actor->field_0x1f0;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_710093b204())) {
        UnkActor* owner = self->owner;
        UnkVec3f pos(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        fn_710094b89c(movement, &pos);
    }
}

struct UnkBase_71003cc474;
struct UnkGooplotRequest_b63a3;

struct UnkBombHitSource_ovb0078 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBombHitData_ovb0078 {
    UnkBombHitSource_ovb0078* source;
    u8 pad_0x8[0xc];
    UnkVec3f point;   // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
    u8 pad_0x38[4];
    u8 flags;         // 0x3c
};

struct UnkBombRequest_ovb0078 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    s32 field_0x68;
    UnkVec3f origin;  // 0x6c
    UnkVec3f dir;     // 0x78
    UnkVec3f back;    // 0x84
    u32 field_0x90;
    u32 field_0x94;
    u8 pad_0x98[0x100 - 0x98];
};

struct UnkBombReceiver_ovb0078 {
    void** vtable;
    s32 vf_0x190(UnkActor* owner, UnkBombRequest_ovb0078* request, UnkBombHitData_ovb0078* hit) {
        return reinterpret_cast<s32 (*)(UnkBombReceiver_ovb0078*, UnkActor*, UnkBombRequest_ovb0078*,
                                        UnkBombHitData_ovb0078*)>(vtable[0x190 / 8])(this, owner, request, hit);
    }
    void vf_0x188(UnkBombRequest_ovb0078* request) {
        reinterpret_cast<void (*)(UnkBombReceiver_ovb0078*, UnkBombRequest_ovb0078*)>(vtable[0x188 / 8])(this,
                                                                                                         request);
    }
};

struct UnkBombTargetActor_ovb0078 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b8 - 0x68];
    UnkBombReceiver_ovb0078* field_0x1b8;
};

struct UnkBombCollideMovement_ovb0078;

struct UnkBombCollideMovementVtbl_ovb0078 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkBombCollideMovement_ovb0078* self, UnkBombTargetActor_ovb0078* actor,
                     UnkBombHitData_ovb0078* hit);
};

struct UnkBombCollideMovement_ovb0078 {
    UnkBombCollideMovementVtbl_ovb0078* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x348 - 0x38];
    UnkBombRequest_ovb0078 request;  // 0x348
};

void fn_7100aabdac(UnkBombCollideMovement_ovb0078* self, UnkBombTargetActor_ovb0078* actor, void*, void*,
                   UnkBombHitData_ovb0078* hit) {
    if (fn_7100ac79d8(self, actor, hit))
        return;
    UnkBombRequest_ovb0078 request;
    fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(&request),
                  reinterpret_cast<const UnkGooplotRequest_b63a3*>(&self->request));
    f32 ax = actor->x, ay = actor->y, az = actor->z;
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = ax - ox;
    f32 dy = ay - oy;
    f32 dz = az - oz;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    UnkVec3f dir(dx * inv, dy * inv, dz * inv);
    UnkVec3f origin = hit->point;
    UnkVec3f back = (hit->flags & 0x10) ? hit->normal : UnkVec3f(-dir.x, -dir.y, -dir.z);
    request.field_0x68 = -1;
    request.origin = origin;
    request.dir = dir;
    request.back = back;
    request.field_0x90 = 0;
    request.field_0x94 = 0x38;
    request.field_0x60 = hit->source ? hit->source->field_0x10 : nullptr;
    if (UnkBombReceiver_ovb0078* receiver = actor->field_0x1b8) {
        if (receiver->vf_0x190(self->owner, &request, hit) != 2) {
            receiver->vf_0x188(&request);
            self->vtbl->vf_0x2a8(self, actor, hit);
        }
    }
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
}

struct UnkBombExplodeMovement_ovb0078;
struct UnkBombExplodeMovementVtbl_ovb0078 {
    void* slots[0x2f0 / 8];
    void (*vf_0x2f0)(UnkBombExplodeMovement_ovb0078* self, UnkBombTargetActor_ovb0078* actor);
};

struct UnkBombExplodeMovement_ovb0078 {
    UnkBombExplodeMovementVtbl_ovb0078* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x348 - 0x38];
    UnkBombRequest_ovb0078 request;  // 0x348
    u8 pad_0x448[0x488 - 0x448];
    bool field_0x488;
};

void fn_7100aaa7d8(UnkBombExplodeMovement_ovb0078* self, UnkBombTargetActor_ovb0078* actor, bool hit) {
    if (self->field_0x488)
        return;
    self->vtbl->vf_0x2f0(self, actor);
    if (!hit)
        return;
    UnkBombRequest_ovb0078 request;
    fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(&request),
                  reinterpret_cast<const UnkGooplotRequest_b63a3*>(&self->request));
    f32 ax = actor->x, ay = actor->y, az = actor->z;
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = ax - ox;
    f32 dy = ay - oy;
    f32 dz = az - oz;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    UnkVec3f dir(dx * inv, dy * inv, dz * inv);
    owner = self->owner;
    UnkVec3f origin(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    UnkVec3f back(-dir.x, -dir.y, -dir.z);
    request.field_0x68 = -1;
    request.origin = origin;
    request.dir = dir;
    request.back = back;
    request.field_0x90 = 0;
    request.field_0x94 = 0x38;
    request.field_0x60 = nullptr;
    if (UnkBombReceiver_ovb0078* receiver = actor->field_0x1b8)
        receiver->vf_0x188(&request);
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
}

struct UnkShape;

struct UnkBombAimMovement_ovb0078 {
    u8 pad_0x0[0x4a0];
    UnkWeakHandle* field_0x4a0;  // the target
    f32 field_0x4a8;             // the angle towards it
};

void fn_7100aac004(UnkBombAimMovement_ovb0078* self, UnkWeakTarget* target) {
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&self->field_0x4a0, handle);
    if (handle && handle->target) {
        UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(handle->target));
        self->field_0x4a8 = atan2f(m.m[5], m.m[1]);
    }
}

extern const u64 g_unk_710164b678 __attribute__((visibility("hidden")));

struct UnkBombHitObject_ovb0078 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* field_0x30;  // the collision component's owner
};

struct UnkBombHitShape_ovb0078 {
    u8 pad_0x0[0x10];
    UnkBombHitObject_ovb0078* field_0x10;
};

struct UnkBombHitEntry_ovb0078 {
    u8 pad_0x0[0x10];
    UnkBombHitShape_ovb0078* field_0x10;
};

struct UnkBombHitInfo_ovb0078 {
    UnkBombHitEntry_ovb0078* field_0x0;
};

struct UnkBombHitHandler_ovb0078 {};

using UnkBombOnActor_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(void* actor, void* arg, const u64* key,
                                                                   UnkBombHitInfo_ovb0078* info);
using UnkBombOnTiles_ovb0078 = void (UnkBombHitHandler_ovb0078::*)(UnkBombHitObject_ovb0078* tiles, void* arg,
                                                                   const u64* key, UnkBombHitInfo_ovb0078* info,
                                                                   f32 value);

void fn_7100aaaaf4(UnkBombHitHandler_ovb0078* self, u32* kinds, void* arg, UnkBombHitInfo_ovb0078* info,
                   f32 value, UnkBombOnActor_ovb0078 on_actor, UnkBombOnTiles_ovb0078 on_tiles) {
    UnkBombHitEntry_ovb0078* entry = info->field_0x0;
    if (!entry || !entry->field_0x10)
        return;
    UnkBombHitObject_ovb0078* object = entry->field_0x10->field_0x10;
    if (!object)
        return;
    if (fn_710009d50c(object->vf_0x0(), fn_710085fc44())) {
        if (kinds)
            *kinds |= 1;
        (self->*on_actor)(object->field_0x30, arg, &g_unk_710164b678, info);
        return;
    }
    if (fn_710009d50c(object->vf_0x0(), fn_7100ca0484())) {
        if (kinds)
            *kinds |= 2;
        (self->*on_tiles)(object, arg, &g_unk_710164b678, info, value);
    }
}
