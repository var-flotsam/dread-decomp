#include "game/UnkTU811.h"
#include "types.h"

struct UnkTutoHandle;

struct CSPBTutoLogicAction {
    u8 unk_0x0[0x11];
    bool field_0x11;
    u8 unk_0x12[0x190 - 0x12];
    bool field_0x190;
    u8 unk_0x191[0x7];
    UnkTutoHandle* handle;  // 0x198
};

// Marks the action done and releases its handle.
bool fn_7100a52ebc(CSPBTutoLogicAction* self) {
    self->field_0x190 = true;
    self->field_0x11 = false;
    UnkTutoHandle** slot = &self->handle;
    if (UnkTutoHandle* handle = *slot) {
        fn_7100565ef0(handle, slot);
        *slot = nullptr;
    }
    return false;
}
