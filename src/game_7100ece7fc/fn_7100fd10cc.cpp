// Constructor of a 0x150-byte entry (eight of them in fn_7100fcfcdc): stores its kind and
// default-initialises two arrays and two blocks of settings, each with an array and an empty
// string id.
#include <new>

#include "game/UnkEntry_0x150.h"
#include "game/UnkTU2636.h"
#include "types.h"

using base::global::CStrId;

// A block of settings with its defaults: an empty array of path records and an empty string id.
static void ConstructBlock(UnkEntryBlock_0x60* block) {
    block->field_0x0 = 0;
    block->field_0x4 = 1;
    block->field_0x8 = 1.0f;
    block->field_0xc = 0;
    block->field_0x10 = 0;
    block->field_0x14 = 1.0f;
    for (bool& flag : block->field_0x18)
        flag = false;
    block->field_0x26 = true;
    new (&block->records) UnkEntryVector<UnkEntryPathRecord_0x18>(0xf);
    block->field_0x50 = nullptr;
    new (&block->id) CStrId("", true);
}

void fn_7100fd10cc(UnkEntry_0x150* self, s32 kind) {
    self->kind = kind;
    self->value.field_0x14 = 1;
    self->value.field_0x0 = nullptr;
    self->value.type = nullptr;
    self->value.kind = 2;
    self->field_0x30 = false;
    self->vtable = &g_unk_7101cac0c0[2];
    self->field_0x8 = nullptr;
    new (&self->field_0x38) UnkEntryVector<void*>(0xf);
    new (&self->field_0x60) UnkEntryVector<void*>(0xf);
    for (UnkEntryBlock_0x60& block : self->blocks)
        ConstructBlock(&block);
}
