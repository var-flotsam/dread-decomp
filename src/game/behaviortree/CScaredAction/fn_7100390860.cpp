// A message handler: for a message that carries a non-empty string id (the one in the message's
// payload, or the default id), switches the handler's state word at +0x30 to 3 (kind-3 message,
// id of the first known hash) or to 5 (kind-1 message, id of the second known hash).
// Always returns 1.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkHashedString {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

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

struct UnkHandler {
    u8 pad_0x0[0x2c];
    u32 field_0x2c;
    // Union members: the original reloads the message kind after the stores.
    union {
        u32 field_0x30;
    };
    union {
        u32 field_0x34;
    };
};

// NON_MATCHING(minor): the two words of the state pair are held in swapped registers (the value
// loaded from 0x2c and the constant 3) at the paired store
u32 fn_7100390860(UnkHandler* self, UnkMessage* message) {
    const CStrId* id = &g_unk_7101d470e0;
    if (message->field_0x8) {
        UnkPayload* inner = message->field_0x8->field_0x10;
        id = inner ? &inner->field_0x38 : id;
    }
    base::global::UnkStringData* data = id->data;
    if (data == &g_unk_7101d47108->empty)
        return 1;
    u32 kind = message->field_0x0;
    if (kind == 3) {
        if (reinterpret_cast<UnkHashedString*>(data)->field_0x30 != 0x14d8371df8520610ull)
            return 1;
        u32 old = self->field_0x2c;
        self->field_0x30 = 3;
        self->field_0x34 = old;
        kind = message->field_0x0;
    }
    if (kind == 1) {
        if (reinterpret_cast<UnkHashedString*>(id->data)->field_0x30 != 0x40cef973c887bdc4ull)
            return 1;
        self->field_0x30 = 5;
    }
    return 1;
}
