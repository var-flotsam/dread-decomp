// CBaseDamageTriggerComponent's constructor (fn_7100a45a30): on the trigger base (fn_7100a46a68), its
// vtable, the damage request at +0xac0 (fn_71008c0c74) and cleared timing fields, with the damage cap
// at +0xbd4 at the largest float. Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101a8e968[];
void fn_7100a46a68(char*);
struct UnkStruct_71008c0c74;
void fn_71008c0c74(UnkStruct_71008c0c74* self);

struct UnkBaseDamageTrigger3 {
    void* vtable;
    u8 unk_0x8[0xac0 - 0x8];
    u8 request[0x100];  // 0xac0
    f32 field_0xbc0;
    f32 field_0xbc4;
    f32 field_0xbc8;
    f32 field_0xbcc;
    f32 field_0xbd0;
    f32 field_0xbd4;
    f32 field_0xbd8;
    f32 field_0xbdc;
    bool field_0xbe0;
};

void fn_7100a45a30(UnkBaseDamageTrigger3* self) {
    fn_7100a46a68(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a8e968 + 0x10;
    fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(self->request));
    self->field_0xbc0 = 0.0f;
    self->field_0xbc4 = 0.0f;
    self->field_0xbc8 = 0.0f;
    self->field_0xbcc = 0.0f;
    self->field_0xbd0 = 0.0f;
    self->field_0xbd4 = 3.40282347e+38f;
    self->field_0xbd8 = 0.0f;
    self->field_0xbdc = 0.0f;
    self->field_0xbe0 = false;
}
