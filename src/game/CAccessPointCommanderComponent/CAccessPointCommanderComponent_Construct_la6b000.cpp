// CAccessPointCommanderComponent's unit: the constructor body (fn_7100a6b000). Runs the base constructor,
// sets the vtable and the handle source's fields, then calls the object's vtable slot 3.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CAccessPointCommanderComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

void fn_7100a6c9f4(void* self);
void fn_7100a6b0cc();
void fn_7100a6b0d8();

struct UnkAccessPointCommander_la6b000 {
    void** vtable;
    u8 pad_0x08[0x998];
    void* field_0x9a0;
    void* field_0x9a8;
    void* field_0x9b0;
    s32 field_0x9b8;
    u8 field_0x9bc;
    s32 field_0x9c0;
    void* field_0x9c8;
    s32 field_0x9d0;
    void* field_0x9d8;
    u8 field_0x9e0;
    u8 field_0x9e1;
    void* field_0x9e8;
    void* field_0x9f0;
    void* field_0x9f8;
    void* field_0xa00;
    void* field_0xa08;
    void* field_0xa10;
    void* field_0xa18;
};
static_assert(__builtin_offsetof(UnkAccessPointCommander_la6b000, field_0xa18) == 0xa18, "layout");

void fn_7100a6b000(UnkAccessPointCommander_la6b000* self) {
    fn_7100a6c9f4(self);
    void* const* vt = reinterpret_cast<void* const*>(g_unk_7101a8fa30 + 0x10);
    void* const* base_vt = g_unk_7101a20998 + 2;
    self->vtable = const_cast<void**>(vt);
    self->field_0x9a0 = const_cast<void**>(base_vt);
    self->field_0x9a8 = nullptr;
    self->field_0x9b0 = nullptr;
    self->field_0x9b8 = 10;
    self->field_0x9bc = 1;
    self->field_0x9c0 = 15;
    self->field_0x9c8 = fn_7100083990();
    self->field_0x9d0 = 0;
    self->field_0x9a0 = g_unk_7101a33fa0 + 2;
    self->field_0x9d8 = nullptr;
    self->field_0x9e8 = nullptr;
    self->field_0x9f0 = self;
    *reinterpret_cast<u16*>(&self->field_0x9e0) = 0x100;
    self->field_0x9f8 = reinterpret_cast<void*>(fn_7100a6b0cc);
    self->field_0xa00 = nullptr;
    self->field_0xa10 = reinterpret_cast<void*>(fn_7100a6b0d8);
    self->field_0xa08 = self;
    self->field_0xa18 = nullptr;
    reinterpret_cast<void (*)(void*)>(self->vtable[3])(self);
}
