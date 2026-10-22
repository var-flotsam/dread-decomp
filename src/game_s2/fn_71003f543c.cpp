// Constructor of a component: runs the base and two member constructors, then sets its defaults (ids
// of -1.0, an empty array of step 10 with flags 0xf, a copy of the default string id) and finishes
// by calling the vtable's slot at +0x18 on itself.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkComponent {
    void** vtable;
    u8 pad_0x8[0x149b0];
    u64 field_0x149b8;
    u8 pad_0x149c0[0x188];
    u8 field_0x14b48;
    u8 pad_0x14b49[3];
    f32 field_0x14b4c;
    f32 field_0x14b50;
    u8 pad_0x14b54[4];
    base::UnkVector<void*> field_0x14b58;
    CStrId field_0x14b80;
    f32 field_0x14b88;
    f32 field_0x14b8c;
    u64 field_0x14b90;
    u64 field_0x14b98;
    u16 field_0x14ba0;
};

extern void* g_unk_7101a31448[];

void fn_71003e5994(UnkComponent* self);
void fn_71003ea504(void* member);
void fn_71003f13fc(void* member);

void fn_71003f543c(UnkComponent* self) {
    fn_71003e5994(self);
    self->vtable = g_unk_7101a31448 + 2;
    self->field_0x149b8 = 0;
    fn_71003ea504(reinterpret_cast<char*>(self) + 0x149c0);
    fn_71003f13fc(reinterpret_cast<char*>(self) + 0x14a20);
    self->field_0x14b48 = 0;
    self->field_0x14b4c = -1.0f;
    self->field_0x14b50 = 0.0f;
    new (&self->field_0x14b58) base::UnkVector<void*>(0xf);
    new (&self->field_0x14b80) CStrId(g_unk_7101d470e0);
    self->field_0x14b88 = -1.0f;
    self->field_0x14b8c = -1.0f;
    self->field_0x14b90 = 0;
    self->field_0x14b98 = 0;
    self->field_0x14ba0 = 0;
    reinterpret_cast<void (*)(UnkComponent*)>(self->vtable[3])(self);
}
