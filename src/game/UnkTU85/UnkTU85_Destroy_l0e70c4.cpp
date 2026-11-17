// UnkTU85 (tools/tu_map.py unit 85): the destructor of a reflected class's 0xb0-byte field record. Its two
// named-value dictionaries (names released, values destroyed) are cleared and freed, its two callbacks
// destroyed and its name released, members last to first. Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

struct UnkCallback_l0e70c4;

// The callback's operations; the slot at +0x40 destroys the callback's storage.
struct UnkCallbackOps_l0e70c4 {
    void* pad_0x0[8];
    void (*destroy)(UnkCallback_l0e70c4* self);  // 0x40
};

struct UnkCallback_l0e70c4 {
    void* storage;
    const UnkCallbackOps_l0e70c4* ops;

    ~UnkCallback_l0e70c4() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

using UnkNamedValues_l0e70c4 = base::global::CRntSmallDictionary<CStrId, UnkReflValue>;

struct UnkFieldRecord_l0e70c4 {
    CStrId name;
    u8 pad_0x8[0x10];
    UnkCallback_l0e70c4 callback_0x18;
    UnkCallback_l0e70c4 callback_0x28;
    UnkNamedValues_l0e70c4 values_0x38;
    u8 pad_0x60[0x10];
    UnkNamedValues_l0e70c4 values_0x70;
    u8 pad_0x98[0x18];
};
static_assert(__builtin_offsetof(UnkFieldRecord_l0e70c4, values_0x38) == 0x38, "layout");
static_assert(__builtin_offsetof(UnkFieldRecord_l0e70c4, values_0x70) == 0x70, "layout");
static_assert(sizeof(UnkFieldRecord_l0e70c4) == 0xb0, "layout");

void fn_71000e70c4(UnkRecordB0_b44a1* record) {
    UnkFieldRecord_l0e70c4* self = reinterpret_cast<UnkFieldRecord_l0e70c4*>(record);
    if (self->values_0x70.keys && self->values_0x70.count) {
        for (u64 i = 0; i < self->values_0x70.count; ++i)
            self->values_0x70.keys[i].Reset();
    }
    if (self->values_0x70.values) {
        for (u32 i = 0; i < self->values_0x70.count; ++i)
            fn_71000a1c60(&self->values_0x70.values[i]);
    }
    self->values_0x70.count = 0;
    if (self->values_0x70.keys)
        self->values_0x70.allocator->vf_0x48(self->values_0x70.keys);
    if (self->values_0x70.values)
        self->values_0x70.allocator->vf_0x48(self->values_0x70.values);
    if (self->values_0x38.keys && self->values_0x38.count) {
        for (u64 i = 0; i < self->values_0x38.count; ++i)
            self->values_0x38.keys[i].Reset();
    }
    if (self->values_0x38.values) {
        for (u32 i = 0; i < self->values_0x38.count; ++i)
            fn_71000a1c60(&self->values_0x38.values[i]);
    }
    self->values_0x38.count = 0;
    if (self->values_0x38.keys)
        self->values_0x38.allocator->vf_0x48(self->values_0x38.keys);
    if (self->values_0x38.values)
        self->values_0x38.allocator->vf_0x48(self->values_0x38.values);
    self->callback_0x28.~UnkCallback_l0e70c4();
    self->callback_0x18.~UnkCallback_l0e70c4();
    self->name.Reset();
}
