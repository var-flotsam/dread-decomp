// CAimComponent: a message handler. When the component's state (+0x50, its word at +0x10 with the
// low bit cleared) is 2 and the message's type hash is 0xf4061ff25a2163d6, kind 2 copies +0x670
// to +0x674 and kind 1 sets the flag at +0x6a2 when the sender's word at +0x14 is zero. Names and
// layouts are placeholders.
#include "types.h"

struct UnkAimState {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkAimMessageType {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAimSenderData {
    u8 pad_0x0[0x14];
    u32 field_0x14;
};

struct UnkAimSender {
    u8 pad_0x0[0x10];
    UnkAimSenderData* data;
};

struct UnkAimMessage {
    u32 kind;
    u8 pad_0x4[0x4];
    UnkAimSender* sender;
    UnkAimMessageType* type;
};

struct UnkAimComponent {
    u8 pad_0x0[0x50];
    UnkAimState* state;
    u8 pad_0x58[0x670 - 0x58];
    u32 field_0x670;
    u32 field_0x674;
    u8 pad_0x678[0x6a2 - 0x678];
    bool field_0x6a2;
};

bool fn_71003546d8(UnkAimComponent* self, const UnkAimMessage* msg) {
    UnkAimState* state = self->state;
    if (state && (state->field_0x10 & ~1u) == 2 && msg->type->hash == 0xf4061ff25a2163d6ull) {
        switch (msg->kind) {
        case 1:
            if (msg->sender->data->field_0x14 == 0)
                self->field_0x6a2 = true;
            break;
        case 2:
            self->field_0x674 = self->field_0x670;
            break;
        }
    }
    return true;
}
