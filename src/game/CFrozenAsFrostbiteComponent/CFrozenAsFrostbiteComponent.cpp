// Constructor of a component in the CFrozenAsFrostbiteComponent unit: the base constructor
// fn_71007f8bc0, the vtable, cleared fields, a link at +0x3c8 (owner and the callback fn_71007f5d24), the default
// -1 values, an empty array at +0x3e8 (step 10, flags 0xf, default allocator), then the
// post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CFrozenComponent.h"
#include "types.h"

extern void* g_unk_7101a73e88[];
bool fn_71007f5d24(void* owner, void*);

struct UnkFrozenAsFrostbiteComponent {
    void** vtable;
    u8 pad_0x8[0x3a8];
    u64 field_0x3b0;
    u64 field_0x3b8;
    f32 field_0x3c0;
    f32 field_0x3c4;
    u64 field_0x3c8;
    UnkFrozenAsFrostbiteComponent* field_0x3d0;
    void (*field_0x3d8)(void*);
    f32 field_0x3e0;
    u64 field_0x3e8;
    u64 field_0x3f0;
    u32 field_0x3f8;
    bool field_0x3fc;
    u32 field_0x400;
    base::UnkAllocator* field_0x408;
};

void fn_71007f5bf4(UnkFrozenAsFrostbiteComponent* self) {
    fn_71007f8bc0(self);
    self->vtable = g_unk_7101a73e88 + 2;
    self->field_0x3b0 = 0;
    self->field_0x3b8 = 0;
    self->field_0x3c0 = 0.0f;
    self->field_0x3c4 = -1.0f;
    self->field_0x3c8 = 0;
    self->field_0x3d0 = self;
    self->field_0x3d8 = reinterpret_cast<void (*)(void *)>(&fn_71007f5d24);
    self->field_0x3e0 = -1.0f;
    self->field_0x3e8 = 0;
    self->field_0x3f0 = 0;
    self->field_0x3f8 = 10;
    self->field_0x3fc = true;
    self->field_0x400 = 0xf;
    self->field_0x408 = fn_7100083990();
    reinterpret_cast<void (*)(UnkFrozenAsFrostbiteComponent*)>(self->vtable[3])(self);
}
