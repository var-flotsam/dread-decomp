// Unit 1110 (tools/tu_map.py, no name hint). fn_71006f3390: the constructor of an object holding two
// polygon collection shapes (fn_7100d62b04 builds each) followed by an id -1, a cleared word, a float
// 1000 and a count 20. Names and layouts are placeholders.
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

extern void* g_unk_7101a6b5e0[];

struct UnkTwoShapeHolder_b42 {
    void** vtable;
    u8 shape_a[0x120];
    u8 shape_b[0x120];
    u64 field_0x248;
    u64 field_0x250;
    f32 field_0x258;
    u32 field_0x25c;
};

// Declared as src/game/UnkScourgeTargetList_Ctor.cpp does.
void fn_71006f3390(char* block) {
    UnkTwoShapeHolder_b42* self = reinterpret_cast<UnkTwoShapeHolder_b42*>(block);
    self->vtable = &g_unk_7101a6b5e0[2];
    fn_7100d62b04(self->shape_a);
    fn_7100d62b04(self->shape_b);
    self->field_0x248 = 0xffffffff;
    self->field_0x250 = 0;
    self->field_0x258 = 1000.0f;
    self->field_0x25c = 20;
}
