// Constructor of a node: runs its base constructor, sets the vtable, names seven string ids,
// initialises its flag and number fields, leaves an id as the empty string and builds a string
// with inline storage.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;
using base::global::UnkStrIdBase;

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};

struct UnkInlineStr {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStrId field_0x30;
    UnkStrId field_0x38;
    UnkStrId field_0x40;
    UnkStrId field_0x48;
    UnkStrId field_0x50;
    UnkStrId field_0x58;
    UnkStrId field_0x60;
    u64 field_0x68[2];
    u8 pad_0x78[4];
    UnkPacked<u64> field_0x7c;
    u32 field_0x84;
    bool field_0x88;
    u8 pad_0x89;
    bool field_0x8a;
    u8 pad_0x8b;
    bool field_0x8c;
    u8 pad_0x8d[3];
    UnkStrIdBase field_0x90;
    UnkInlineStr field_0x98;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a24d08[];
extern void* g_unk_7101a194b8[];

void fn_710036f678(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a24d08[2];
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) UnkStrId("", true);
    new (&self->field_0x40) UnkStrId("", true);
    new (&self->field_0x48) UnkStrId("", true);
    new (&self->field_0x50) UnkStrId("", true);
    new (&self->field_0x58) UnkStrId("", true);
    new (&self->field_0x60) UnkStrId("", true);
    *reinterpret_cast<u16*>(&self->pad_0x78) = 0x100;
    self->field_0x68[0] = 0;
    self->field_0x68[1] = 0;
    self->field_0x7c.v = -1;
    self->field_0x84 = -1;
    self->field_0x88 = false;
    self->field_0x8a = false;
    self->field_0x8c = false;
    self->field_0x90.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x90.data = &g_unk_7101d47108->empty;
    fn_710007e6e4(reinterpret_cast<base::global::CRntString*>(&self->field_0x98),
                  reinterpret_cast<base::UnkAllocator*>(&self->field_0x98.vtable));
    self->field_0x98.vtable = &g_unk_7101a194b8[2];
    self->field_0x98.field_0x138 = 0;
    self->field_0x98.field_0x140 = 0;
}
