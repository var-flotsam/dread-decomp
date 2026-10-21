// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStrId field_0x30;
    u8 field_0x38;
    u8 pad_0x39[0x3];
    UnkPacked<u64> field_0x3c;
    u8 pad_0x44[0x4];
    UnkStrId field_0x48;
    UnkStrId field_0x50;
    u64 field_0x58;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a26c00[];

void fn_71003927e8(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a26c00[2];
    new (&self->field_0x30) UnkStrId("", true);
    self->field_0x38 = 1;
    self->field_0x3c.v = 0;
    new (&self->field_0x48) UnkStrId("", true);
    new (&self->field_0x50) UnkStrId("", true);
    self->field_0x58 = 2;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
