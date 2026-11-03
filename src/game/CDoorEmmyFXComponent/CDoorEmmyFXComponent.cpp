// Constructor of a component in the CDoorEmmyFXComponent unit: the component base constructor
// fn_71010c4148, the vtable, a pair of floats (-1, 75) at +0x40, a link at +0x58 (owner and the callback
// fn_71007b70d8), the member at +0x70 (fn_710002f4e4), cleared fields, then the class-entry pass of
// base::core::CBaseObject (fn_7100023854). Names and layouts are placeholders.
#include "types.h"

struct UnkStateObject;
struct UnkNode;
void fn_71010c4148(UnkStateObject* self);
void fn_710002f4e4(void* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a70870[];
void fn_71007b70d8(void* owner);

struct UnkDoorEmmyFXComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    u64 field_0x48;
    u8 pad_0x50[0x8];
    u64 field_0x58;
    UnkDoorEmmyFXComponent* field_0x60;
    void (*field_0x68)(void*);
    u8 field_0x70[0xb0];
    u64 field_0x120;
    u32 field_0x128;
    u64 field_0x130;
};

void fn_71007b7024(UnkDoorEmmyFXComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->field_0x40 = -1.0f;
    self->field_0x44 = 75.0f;
    self->field_0x48 = 0;
    self->vtable = g_unk_7101a70870 + 2;
    self->field_0x58 = 0;
    self->field_0x60 = self;
    self->field_0x68 = fn_71007b70d8;
    fn_710002f4e4(self->field_0x70);
    self->field_0x120 = 0;
    self->field_0x128 = 0;
    self->field_0x130 = 0;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
