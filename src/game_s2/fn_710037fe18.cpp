// Constructor: runs the base constructor, sets the vtable, makes the string id at 0x30 the empty
// string (creating the pool first when there is none), clears the state at 0x39..0x48, builds the
// sub-object at 0x50 and clears the trailing record at 0x12ad8 (a pair of words and a byte).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkTrailer {
    u64 field_0x0;
    u64 field_0x8;
    u8 field_0x10;
};

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStringData* field_0x30;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a[2];
    u8 field_0x3c[13];
    u8 pad_0x49[7];
    u8 field_0x50[0x12a88];
    UnkTrailer field_0x12ad8;
};

void fn_7100393e68(UnkNode* self);
void fn_71002acfe4(void* sub);
extern void* g_unk_7101a25e50[];

void fn_710037fe18(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25e50 + 2;
    self->field_0x30 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x30 = &g_unk_7101d47108->empty;
    self->field_0x39 = 0;
    __builtin_memset(self->field_0x3c, 0, 13);
    fn_71002acfe4(self->field_0x50);
    self->field_0x12ad8.field_0x10 = 0;
    self->field_0x12ad8.field_0x0 = 0;
    self->field_0x12ad8.field_0x8 = 0;
}
