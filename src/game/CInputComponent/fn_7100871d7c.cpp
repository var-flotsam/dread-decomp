// From the CInputComponent unit: clears the component's input state: the pair at +0x9e0, the word at
// +0x3c of each listener in the list at +0xab0, and the five flags at +0xb04 when any is set. Names
// and layouts are placeholders.
#include "types.h"

struct UnkInputListener {
    u8 pad_0x0[0x3c];
    u32 field_0x3c;
};

struct UnkInputListenerNode {
    UnkInputListener* listener;
    UnkInputListenerNode* next;
};

struct UnkInputComponentState {
    u8 pad_0x0[0x9e0];
    u64 field_0x9e0;
    u32 field_0x9e8;
    u8 pad_0x9ec[0xc4];
    UnkInputListenerNode* listeners;  // 0xab0
    u8 pad_0xab8[0x4c];
    bool flags[5];  // 0xb04
};
static_assert(__builtin_offsetof(UnkInputComponentState, flags) == 0xb04, "UnkInputComponentState layout");

void fn_7100871d7c(UnkInputComponentState* self) {
    UnkInputListenerNode* node = self->listeners;
    self->field_0x9e8 = 0;
    self->field_0x9e0 = 0;
    for (; node; node = node->next)
        node->listener->field_0x3c = 0;
    bool* flags = self->flags;
    if (flags[0] || flags[4] || flags[3] || flags[1] || flags[2]) {
        // The flags' address is kept: the five are cleared through it (the last one first).
        asm("" : "+r"(flags));
        flags[4] = false;
        *reinterpret_cast<u32*>(flags) = 0;
    }
}
