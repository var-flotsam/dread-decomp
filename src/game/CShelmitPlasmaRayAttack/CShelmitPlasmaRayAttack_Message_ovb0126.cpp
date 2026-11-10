// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): fn_71006b6540 handles a message (kind at +0x0, the
// sender's id at +0x38 of the object at +0x10 of +0x8, the empty id g_unk_7101d470e0 when there is none)
// while the owner (+0x8) has its CShelmitAIComponent (+0x240) and the attack its object at +0x28: kind 3 with
// id 0x52b2c974f41e0eae runs fn_71006b430c when fn_71006b5c00 allows it; kind 1 with that id moves to state 9
// (+0x30) and copies the object's word at +0x100 to +0xa8, with ids 0x058e983ee43b033a or 0x9f65ed8ea84722af
// to state 8. Always returns true. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkPlasmaRaySender_ovb0126 {
    u8 pad_0x0[0x38];
    CStrId id;
};

struct UnkPlasmaRaySenderRef_ovb0126 {
    u8 pad_0x0[0x10];
    UnkPlasmaRaySender_ovb0126* sender;
};

struct UnkPlasmaRayMessage_ovb0126 {
    u32 kind;
    u8 pad_0x4[4];
    UnkPlasmaRaySenderRef_ovb0126* ref;
};

struct UnkPlasmaRayOwner_ovb0126 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;
};

struct UnkPlasmaRayTarget_ovb0126 {
    u8 pad_0x0[0x100];
    u32 field_0x100;
};

struct UnkPlasmaRayAttack_ovb0126 {
    u8 pad_0x0[0x8];
    UnkPlasmaRayOwner_ovb0126* owner;
    u8 pad_0x10[0x18];
    UnkPlasmaRayTarget_ovb0126* target;  // 0x28
    u32 state;  // 0x30
    u8 pad_0x34[0xa8 - 0x34];
    u32 field_0xa8;
};

bool fn_71006b5c00(UnkPlasmaRayAttack_ovb0126* self);
void fn_71006b430c(UnkPlasmaRayAttack_ovb0126* self);

static inline u64 Hash(const CStrId* id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id->data) + 0x30);
}

bool fn_71006b6540(UnkPlasmaRayAttack_ovb0126* self, const UnkPlasmaRayMessage_ovb0126* message) {
    const CStrId* id = &g_unk_7101d470e0;
    if (message->ref) {
        UnkPlasmaRaySender_ovb0126* sender = message->ref->sender;
        id = sender ? &sender->id : &g_unk_7101d470e0;
    }
    UnkPlasmaRayOwner_ovb0126* owner = self->owner;
    if (!owner)
        return true;
    UnkReflObject* ai = owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71006b98c0()))
        return true;
    if (!self->target || id->data == &g_unk_7101d47108->empty)
        return true;
    switch (message->kind) {
    case 1:
        switch (Hash(id)) {
        case 0x9f65ed8ea84722af:
            self->state = 8;
            break;
        case 0x52b2c974f41e0eae:
            self->state = 9;
            self->field_0xa8 = self->target->field_0x100;
            break;
        case 0x058e983ee43b033a:
            self->state = 8;
            break;
        }
        break;
    case 3:
        if (Hash(id) == 0x52b2c974f41e0eae && fn_71006b5c00(self))
            fn_71006b430c(self);
        break;
    }
    return true;
}
