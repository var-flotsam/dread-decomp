// CSamusMovementState::CParams unit (tools/tu_map.py, reflection hint). fn_71009b0580: takes a
// parameter block unless its 64-bit id is -1: the id and the seven words after it (+0x0, copied as
// one 4-aligned block), then two more words (+0x24, +0x28), stored from +0x8 on. Names and layouts
// are placeholders.
#include "types.h"

struct UnkParamHead_b42 {
    u32 id[2];
    u32 words[7];
};

struct UnkParamBlock_b42 {
    UnkParamHead_b42 head;
    u32 field_0x24;
    u32 field_0x28;
};

struct UnkParamHolder_b42 {
    u8 pad_0x0[0x8];
    UnkParamBlock_b42 block;  // 0x8
};

void fn_71009b0580(UnkParamHolder_b42* self, const UnkParamBlock_b42* block) {
    if (*reinterpret_cast<const s64*>(block->head.id) == -1)
        return;
    UnkParamBlock_b42& b = self->block;
    b.head = block->head;
    b.field_0x24 = block->field_0x24;
    b.field_0x28 = block->field_0x28;
}
