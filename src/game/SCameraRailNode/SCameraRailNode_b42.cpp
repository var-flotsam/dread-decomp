// SCameraRailNode's unit (tools/tu_map.py, the name from reflection): the reflected constructor and
// copy constructor of a camera-rail node: its vtable, three words and a callback set (UnkCallbackSet,
// include/game/UnkCallbackSetVector.h) copied by its own copy constructor. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkCallbackSetVector.h"
#include "types.h"

extern char g_unk_7101aa2888[];
extern void* g_unk_7101a20998[];  // the callback set's base vtable
extern void* g_unk_7101a20950[];  // the callback set's vtable

struct UnkCameraRailNode_b42a3 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkCallbackSet set;
};

// Signature as src/reflection/SCameraRailNode.cpp declares it.
void fn_7100c0778c(void* dst, const void* src) {
    auto* self = static_cast<UnkCameraRailNode_b42a3*>(dst);
    auto* other = static_cast<const UnkCameraRailNode_b42a3*>(src);
    self->vtable = g_unk_7101aa2888 + 0x10;
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    self->field_0x10 = other->field_0x10;
    new (&self->set) UnkCallbackSet(other->set);
}

// The callback set's default construction, written out (the header's type has no default
// constructor): through its base (vtable, an empty array with allocator flags 0xf, a cleared word),
// then its own vtable and no owner.
struct UnkCallbackSetRaw_b42a3 {
    void* vtable;
    base::UnkVector<void*> callbacks;
    u32 field_0x30;
    void* owner;
};

struct UnkCameraRailNodeRaw_b42a3 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkCallbackSetRaw_b42a3 set;
};

void fn_7100c07710(UnkCameraRailNodeRaw_b42a3* self) {
    self->vtable = g_unk_7101aa2888 + 0x10;
    self->field_0x8 = 0;
    self->field_0xc = 0;
    self->field_0x10 = 0;
    self->set.vtable = g_unk_7101a20998 + 2;
    new (&self->set.callbacks) base::UnkVector<void*>(0xf);
    self->set.field_0x30 = 0;
    self->set.vtable = g_unk_7101a20950 + 2;
    self->set.owner = nullptr;
}
