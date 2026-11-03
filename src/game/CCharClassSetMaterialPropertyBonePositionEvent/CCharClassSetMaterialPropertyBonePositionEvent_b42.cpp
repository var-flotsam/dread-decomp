// CCharClassSetMaterialPropertyBonePositionEvent's unit (tools/tu_map.py): its constructor, the shape of
// fn_7100cbe13c (CCharClassFXFadeEvent): the base constructor (fn_7100067ea4), the vtable, four string
// ids built from "", then the event's fields (a word pair (0, 1), a cleared pointer, a float of 1 and a
// cleared flag). Names and the layout are placeholders.
#include "types.h"

// As the other event constructors declare them (src/game/CCharClassFXFadeEvent/).
void fn_71000003d4(void* self, const char* s, bool flag);
void fn_7100067ea4(void* self);  // the base constructor
extern char g_unk_7101aa1b88[];

struct UnkBonePositionEvent_b42a3 {
    char* vtable;
    u8 unk_0x8[0x8];
    u64 ids[4];
    u32 field_0x30;
    u32 field_0x34;
    u64 field_0x38;
    f32 field_0x40;
    bool field_0x44;
};

void fn_7100bd85e4(UnkBonePositionEvent_b42a3* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1b88 + 0x10;
    fn_71000003d4(&self->ids[0], "", true);
    fn_71000003d4(&self->ids[1], "", true);
    fn_71000003d4(&self->ids[2], "", true);
    fn_71000003d4(&self->ids[3], "", true);
    self->field_0x30 = 0;
    self->field_0x34 = 1;
    self->field_0x38 = 0;
    self->field_0x40 = 1.0f;
    self->field_0x44 = false;
}
