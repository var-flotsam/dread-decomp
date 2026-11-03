// CCharClassDoorLifeComponent: its constructor (fn_71008b2cb4). The base constructor fn_71008b4c88, a
// flag at +0x11c, the floats 0 and 300 at +0x120, the vtable, seven cleared bytes at +0x128, eight
// float defaults from +0x130, then the class-entry pass of base::core::CBaseObject (fn_7100023854).
// fn_71008b2bbc is the same without the class-entry pass (the vtable stored first). Names and layouts
// are placeholders.
#include "types.h"

struct UnkNode;
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a7dd50[];

struct UnkDoorLifeComponent {
    void** vtable;
    u8 pad_0x8[0x114];
    bool field_0x11c;
    u8 pad_0x11d[3];
    f32 field_0x120;
    f32 field_0x124;
    u8 field_0x128[7];
    u8 pad_0x12f;
    f32 field_0x130[8];
};

void fn_71008b4c88(UnkDoorLifeComponent* self);

static inline __attribute__((always_inline)) void InitDefaults(UnkDoorLifeComponent* self) {
    self->field_0x11c = true;
    self->field_0x120 = 0.0f;
    self->field_0x124 = 300.0f;
}

static inline __attribute__((always_inline)) void InitFloats(UnkDoorLifeComponent* self) {
    __builtin_memset(self->field_0x128, 0, sizeof(self->field_0x128));
    self->field_0x130[0] = 1.0f;
    self->field_0x130[1] = 0.8f;
    self->field_0x130[2] = 0.34f;
    self->field_0x130[3] = 1.0f;
    self->field_0x130[4] = 0.1f;
    self->field_0x130[5] = 0.1f;
    self->field_0x130[6] = 0.1f;
    self->field_0x130[7] = 1.0f;
}

void fn_71008b2cb4(UnkDoorLifeComponent* self) {
    fn_71008b4c88(self);
    InitDefaults(self);
    self->vtable = g_unk_7101a7dd50 + 2;
    InitFloats(self);
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}

void fn_71008b2bbc(UnkDoorLifeComponent* self) {
    fn_71008b4c88(self);
    self->vtable = g_unk_7101a7dd50 + 2;
    InitDefaults(self);
    InitFloats(self);
}
