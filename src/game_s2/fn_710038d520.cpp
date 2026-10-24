// A message handler: for a message that carries a non-empty string id (the one in the message's
// payload, or the default id), runs fn_710038cd5c when a kind-1 message's id is the one stored at
// +0x70, or when a kind-2 message arrives while the owner's attack at +0x220 is a graze track.
// Always returns 1.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkAttack;

struct UnkPayload {
    u8 pad_0x0[0x10];
    UnkPayload* field_0x10;
    u8 pad_0x18[0x20];
    CStrId field_0x38;
};

struct UnkMessage {
    u32 field_0x0;
    u8 pad_0x4[4];
    UnkPayload* field_0x8;
};

struct UnkOwner {
    u8 pad_0x0[0x220];
    UnkAttack* field_0x220;
};

struct UnkHandler {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
    u8 pad_0x20[0x50];
    base::global::UnkStringData* field_0x70;
};

bool fn_710117b348(UnkAttack* object, const base::reflection::CClass* class_ptr);
void fn_710038cd5c(UnkHandler* self);

u32 fn_710038d520(UnkHandler* self, UnkMessage* message) {
    const CStrId* id = &g_unk_7101d470e0;
    if (message->field_0x8) {
        UnkPayload* inner = message->field_0x8->field_0x10;
        id = inner ? &reinterpret_cast<UnkPayload*>(inner)->field_0x38 : id;
    }
    base::global::UnkStringData* data = id->data;
    if (data == &g_unk_7101d47108->empty)
        return 1;
    u32 kind = message->field_0x0;
    if (kind != 2) {
        if (kind != 1 || data != self->field_0x70)
            return 1;
    } else {
        UnkAttack* attack = self->field_0x18->field_0x220;
        if (!attack || !fn_710117b348(attack, fn_7100c219b8()))
            return 1;
    }
    fn_710038cd5c(self);
    return 1;
}
