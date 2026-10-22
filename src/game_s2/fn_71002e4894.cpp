// Constructor of an object derived from fn_71002dba08's: sets its vtable, clears 0x28..0x37, builds
// a growable array at +0x38 (flags 0x40) with room for two elements, clears +0x60 and +0x68, sets
// its final vtable and finishes in fn_7100023854.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x20];
    u64 field_0x28;
    u64 field_0x30;
    base::UnkVector<void*> field_0x38;
    u64 field_0x60;
    u32 field_0x68;
};

void fn_71002dba08(UnkNode*);
void fn_7100023854(UnkNode*);
extern void* g_unk_7101a21948[];
extern void* g_unk_7101a21d78[];

void fn_71002e4894(UnkNode* self) {
    fn_71002dba08(self);
    self->vtable = g_unk_7101a21948 + 2;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    new (&self->field_0x38) base::UnkVector<void*>(0x40);
    self->field_0x38.Reserve(2);
    self->field_0x60 = 0;
    self->vtable = g_unk_7101a21d78 + 2;
    self->field_0x68 = 0;
    fn_7100023854(self);
}
