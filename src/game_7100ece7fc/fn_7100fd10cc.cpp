// Constructor of a 0x150-byte entry (eight of them in fn_7100fcfcdc): stores its kind and
// default-initialises two arrays and two blocks of settings, each with an array and an empty
// string id.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrIdBase;

struct UnkEntryBlock_0x60 {
    u32 field_0x0;
    u32 field_0x4;
    f32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    f32 field_0x14;
    bool field_0x18[14];
    bool field_0x26;
    base::UnkVector<void*> field_0x28;
    void* field_0x50;
    UnkStrIdBase field_0x58;

    __attribute__((always_inline)) UnkEntryBlock_0x60()
        : field_0x0(0), field_0x4(1), field_0x8(1.0f), field_0xc(0), field_0x10(0), field_0x14(1.0f),
          field_0x18(), field_0x26(true), field_0x28(0xf), field_0x50(nullptr), field_0x58("", true) {}
};

struct UnkEntry_0x150 {
    void** vtable;
    void* field_0x8;
    s32 kind;
    struct {
        void* field_0x0;
        void* field_0x8;
    } field_0x18;
    u32 field_0x28;
    u32 field_0x2c;
    bool field_0x30;
    base::UnkVector<void*> field_0x38;
    base::UnkVector<void*> field_0x60;
    UnkEntryBlock_0x60 blocks[2];  // +0x88
};

extern void* g_unk_7101cac0c0[];

void fn_7100fd10cc(UnkEntry_0x150* self, s32 kind) {
    self->kind = kind;
    self->field_0x2c = 1;
    self->field_0x18 = {};
    self->field_0x28 = 2;
    self->field_0x30 = false;
    self->vtable = &g_unk_7101cac0c0[2];
    self->field_0x8 = nullptr;
    new (&self->field_0x38) base::UnkVector<void*>(0xf);
    new (&self->field_0x60) base::UnkVector<void*>(0xf);
    new (self->blocks) UnkEntryBlock_0x60[2];
}
