// CCharClassSoundProofTriggerComponent: its constructor (fn_7100a308b0). The base constructor
// fn_7100a46a68, an allocator-backed list at +0xac0 (two cleared words, three small defaults and the
// allocator from fn_7100083990), the class vtable, a word 9, two floats 1.0, a cleared flag, then
// the object's own vf_0x18. Names and layouts are placeholders.
#include "game/CCharClassBaseTriggerComponent.h"
#include "types.h"

#include "base/UnkVector.h"

extern void* g_unk_7101a8e0f8[];

struct UnkSoundProofTrigger {
    void** vtable;
    u8 pad_0x8[0xab8];
    u64 field_0xac0;
    u64 field_0xac8;
    u32 field_0xad0;
    bool field_0xad4;
    u32 field_0xad8;
    base::UnkAllocator* field_0xae0;
    u32 field_0xae8;
    f32 field_0xaec;
    f32 field_0xaf0;
    bool field_0xaf4;
};

void fn_7100a308b0(UnkSoundProofTrigger* self) {
    fn_7100a46a68(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a8e0f8 + 2;
    self->field_0xac0 = 0;
    self->field_0xac8 = 0;
    self->field_0xad0 = 10;
    self->field_0xad4 = true;
    self->field_0xad8 = 15;
    self->field_0xae0 = fn_7100083990();
    self->field_0xae8 = 9;
    self->field_0xaec = 1.0f;
    self->field_0xaf0 = 1.0f;
    self->field_0xaf4 = false;
    reinterpret_cast<void (*)(UnkSoundProofTrigger*)>(self->vtable[3])(self);
}
