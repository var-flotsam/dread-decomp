// Unit 1161 (tools/tu_map.py, no name hint): fills a small record (three pointers, an id, a scale of 1).
// Names are placeholders.
#include "types.h"

struct UnkRecord_A5_b65 {
    u64 field_0x0;
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;
    u32 pad_0x24;
    f32 field_0x28;
};

void fn_7100745bc4(UnkRecord_A5_b65* self, u32 id, void* a, void* b, void* c) {
    self->field_0x20 = id;
    self->field_0x8 = a;
    self->field_0x10 = b;
    self->field_0x18 = c;
    self->field_0x28 = 1.0f;
}
