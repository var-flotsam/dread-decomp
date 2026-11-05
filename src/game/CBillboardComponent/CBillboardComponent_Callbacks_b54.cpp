// CBillboardComponent unit (tools/tu_map.py, the name from reflection): two optional callbacks, each a
// function pointer with its context stored just before it (+0x58/+0x60 and +0x68/+0x70). Each calls the
// callback with its context and the two arguments when it is set, and returns false otherwise. Names and
// layouts are placeholders.
#include "types.h"

typedef bool (*UnkCallback_b54)(void* context, void* a, void* b);

struct UnkCallbackSlot_b54 {
    void* context;
    UnkCallback_b54 fn;
};

struct UnkBillboardCallbacks_b54 {
    u8 unk_0x0[0x58];
    UnkCallbackSlot_b54 slot_0x58;
    UnkCallbackSlot_b54 slot_0x68;
};

bool fn_710072b554(UnkBillboardCallbacks_b54* self, void* a, void* b) {
    if (UnkCallback_b54 fn = self->slot_0x58.fn)
        return fn(self->slot_0x58.context, a, b);
    return false;
}

bool fn_710072b56c(UnkBillboardCallbacks_b54* self, void* a, void* b) {
    if (UnkCallback_b54 fn = self->slot_0x68.fn)
        return fn(self->slot_0x68.context, a, b);
    return false;
}
