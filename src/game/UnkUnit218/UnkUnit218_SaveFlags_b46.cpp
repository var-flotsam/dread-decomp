// Unit 218 of tools/tu_map.py (no class hint): for every node of the list at +0x160 (next at
// +0x10), the object at +8 of the node keeps its flag at +8 as the previous value at +9; when
// `clear` is set, the flag is also cleared. Names and layouts are placeholders.
#include "types.h"

struct UnkFlagPair_b46a1 {
    u8 pad_0x0[8];
    bool current;
    bool previous;
};

struct UnkFlagNode_b46a1 {
    u8 pad_0x0[8];
    UnkFlagPair_b46a1* object;
    UnkFlagNode_b46a1* next;
};

struct UnkUnit218_b46a1 {
    u8 pad_0x0[0x160];
    UnkFlagNode_b46a1* head;
};

void fn_71001995e8(UnkUnit218_b46a1* self, bool clear) {
    UnkFlagNode_b46a1* node = self->head;
    if (clear) {
        for (; node; node = node->next) {
            UnkFlagPair_b46a1* object = node->object;
            bool value = object->current;
            object->current = false;
            object->previous = value;
        }
    } else {
        for (; node; node = node->next) {
            UnkFlagPair_b46a1* object = node->object;
            object->previous = object->current;
        }
    }
}
