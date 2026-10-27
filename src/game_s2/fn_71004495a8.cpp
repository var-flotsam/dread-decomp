// Constructor of a node: runs its base constructor, sets the vtable, builds an empty string at
// +0x30, leaves an id as the empty string, clears the remaining fields and gives an embedded
// listener its vtable.
#include "base/UnkNamedRegistry.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStrIdBase;

template <class T>
struct __attribute__((packed)) UnkPacked {
    T v;
};

struct UnkListener {
    void** vtable;
    u64 field_0x8;
};

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    base::UnkNamedEntry field_0x30;
    UnkStrIdBase field_0x60;
    u16 field_0x68;
    u8 pad_0x6a[10];
    UnkPacked<u64> field_0x74;
    UnkPacked<u64> field_0x7c;
    u8 pad_0x84[4];
    UnkListener field_0x88;
    u64 field_0x98;
    u64 field_0xa0;
    u8 pad_0xa8[8];
    u64 field_0xb0;
    u64 field_0xb8;
    u32 field_0xc0;
};

extern void* g_unk_7101a36838[];
extern void* g_unk_7101a22d48[];  // the listener's vtable

void fn_71004495a8(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a36838[2];
    fn_710007e704(&self->field_0x30, "");
    self->field_0x60.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x60.data = &g_unk_7101d47108->empty;
    self->field_0x68 = 1;
    self->field_0x74.v = 0;
    self->field_0x7c.v = 0;
    self->field_0x88.vtable = &g_unk_7101a22d48[2];
    self->field_0x88.field_0x8 = 0;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 0;
}
