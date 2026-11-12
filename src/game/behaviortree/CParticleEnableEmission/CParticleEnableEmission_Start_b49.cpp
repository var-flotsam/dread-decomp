// behaviortree::CParticleEnableEmission (tools/tu_map.py): the node's start. After the base routine
// (fn_7100394bd0) it clears +0x3b and, when the effect name (+0x30) is set and the owner (+0x18) has a
// world (+0x280) in which the named effect (by its hash, +0x30 of the name's data) is live, keeps it
// (+0x40), switches its emission on (fn_7100146dac) or off (fn_7100146e60) after +0x38, and marks
// +0x3b; otherwise forgets it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
struct UnkWorld;
struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
struct UnkObject;
bool fn_710114e668(UnkObject*, u8);

struct UnkEmissionPart_b49 {
    u8 pad_0x0[0xb];
    u8 id;
};

struct UnkEmissionOwner_b49 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};

struct UnkParticleEnableEmission_b49 {
    u8 pad_0x0[0x18];
    UnkEmissionOwner_b49* owner;
    u8 pad_0x20[0x10];
    UnkStrId name;
    bool enable;
    u8 pad_0x39[2];
    bool done;
    u8 pad_0x3c[4];
    UnkEmissionPart_b49* part;
};

void fn_71003784d0(UnkParticleEnableEmission_b49* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->done = false;
    if (self->name.data == &g_unk_7101d47108->empty || !self->owner)
        return;
    UnkWorld* world = self->owner->world;
    if (!world)
        return;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self->name.data) + 0x30);
    auto* part = reinterpret_cast<UnkEmissionPart_b49*>(fn_7101152b90(world, hash));
    if (part && fn_710114e668(reinterpret_cast<UnkObject*>(world), part->id)) {
        self->part = part;
        if (self->enable)
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(part), true);
        else
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
        self->done = true;
        return;
    }
    self->part = nullptr;
}
