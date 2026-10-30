// From the CInputComponent unit: clears the component's input state: the pair at +0x9e0, the word at
// +0x3c of each listener in the list at +0xab0, and the five flags at +0xb04 when any is set. Names
// and layouts are placeholders.
#include "base/UnkVector.h"
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
    u8 pad_0x9ec[0xbc];
    union {
        u32 listener_count;  // 0xaa8; a union member, so it is reread after the node stores
    };
    UnkInputListenerNode* listeners;  // 0xab0
    UnkInputListenerNode* last;       // 0xab8
    base::UnkAllocator* allocator;    // 0xac0
    u32 alloc_flags;                  // 0xac8
    u8 pad_0xacc[0x38];
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

// An identical copy (fn_71008721e4).
void fn_71008721e4(UnkInputComponentState* self) {
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

// Adds `listener` to the end of the list unless it is there already; says whether it was added.
// NON_MATCHING(minor): same behaviour; the original joins the two branches at one store of the count
// and lays the not-found return out first.
bool fn_7100872860(UnkInputComponentState* self, UnkInputListener* listener) {
    UnkInputListenerNode* node = self->listeners;
    for (;;) {
        if (!node) {
            node = static_cast<UnkInputListenerNode*>(
                self->allocator->vf_0x40(sizeof(UnkInputListenerNode), 8, self->alloc_flags));
            if (node) {
                node->listener = listener;
                node->next = nullptr;
                if (self->listener_count == 0) {
                    self->last = node;
                    self->listeners = node;
                    self->listener_count = 1;
                } else {
                    self->last->next = node;
                    node->next = nullptr;
                    self->last = node;
                    ++self->listener_count;
                }
            }
            return true;
        }
        if (node->listener == listener)
            break;
        node = node->next;
    }
    return false;
}
