// CProfessorDoorComponent: its constructor (fn_71007ddd8c). The component base constructor
// fn_71010c4148, the base class's vtable, the string-id list at +0x40 (fn_71007d762c), three cleared
// flags at +0x78, then its own vtable and the class-entry pass of base::core::CBaseObject
// (fn_7100023854). Names and layouts are placeholders.
#include "types.h"

struct UnkStateObject;
struct UnkNode;
void fn_71010c4148(UnkStateObject* self);
void fn_71007d762c(char* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a72d70[];
extern void* g_unk_7101a72ff8[];

struct UnkProfessorDoorComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 field_0x40[0x38];
    bool field_0x78;
    bool field_0x79;
    bool field_0x7a;
};

void fn_71007ddd8c(UnkProfessorDoorComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a72d70 + 2;
    fn_71007d762c(reinterpret_cast<char*>(self->field_0x40));
    self->field_0x78 = false;
    self->field_0x79 = false;
    self->field_0x7a = false;
    self->vtable = g_unk_7101a72ff8 + 2;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
