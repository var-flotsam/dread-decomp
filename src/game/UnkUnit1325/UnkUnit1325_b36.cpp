// Unit 1325 of tools/tu_map.py. Names and layouts are placeholders.
#include "types.h"

struct UnkTimedNode {
    u8 pad_0x0[0x8];
    f32 timer;  // 0x8; negative while inactive
    u8 pad_0xc[0x3c];
    UnkTimedNode* next;  // 0x48
};

struct UnkTimedList {
    u8 pad_0x0[0x10];
    UnkTimedNode* head;  // 0x10
};

// Removes `node` and returns the one after it.
UnkTimedNode* fn_710086f718(UnkTimedList* self, UnkTimedNode* node);

// Counts the active nodes' timers down by `dt`, removing each one that runs out.
void fn_710086f6a8(UnkTimedList* self, f32 dt) {
    UnkTimedNode* node = self->head;
    while (node) {
        if (node->timer >= 0.0f) {
            node->timer -= dt;
            if (node->timer <= 0.0f) {
                node = fn_710086f718(self, node);
                asm("" : "+r"(node));
                continue;
            }
        }
        node = node->next;
    }
}
