// CActorComponent's unit (tools/tu_map.py): the copy assignment of a flat map of 8-byte values
// (+0) keyed by string ids (+8), count at +0x10 and capacity at +0x14 (fn_7100251e10; the same
// code as CComponent's fn_71010c6a10, src/game/CComponent/CComponent_KeyedMap_ovb0062.cpp, with
// this unit's own helpers), and a component's enable toggle (fn_710024f918). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkKeyedStrIdMapA_ovb0062 {
    u64* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
};

void fn_7100251f78(UnkKeyedStrIdMapA_ovb0062* self);
void fn_7100248ce8(UnkKeyedStrIdMapA_ovb0062* self, u32 capacity);  // the keyed-array reserve

UnkKeyedStrIdMapA_ovb0062* fn_7100251e10(UnkKeyedStrIdMapA_ovb0062* self, const UnkKeyedStrIdMapA_ovb0062* other) {
    if (self == other)
        return self;
    if (self->keys && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].Reset();
    self->count = 0;
    if (other->count != self->capacity) {
        fn_7100251f78(self);
        fn_7100248ce8(self, other->count);
    }
    __builtin_memcpy(self->values, other->values, (u64)other->count * 8);
    self->count = other->count;
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        // Copy-constructs the key; the pointer is stored as bytes, so the count is reloaded.
        UnkStringData* data = other->keys[i].data;
        __builtin_memcpy(&self->keys[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    return self;
}

f32 fn_7100253ad4(void* source);

struct UnkToggleOwner_ovb0062 {
    u8 unk_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 unk_0x1b1[3];
    bool field_0x1b4;
};

struct UnkToggle_ovb0062 {
    void** vtable;
    u8 unk_0x8[0x8];
    void* source;                   // 0x10
    UnkToggleOwner_ovb0062* owner;  // 0x18
    bool enabled;                   // 0x20
    f32 current;                    // 0x24
    f32 target;                     // 0x28

    bool vf_0x120() { return reinterpret_cast<bool (*)(UnkToggle_ovb0062*)>(vtable[0x120 / 8])(this); }
    void vf_0x110() { reinterpret_cast<void (*)(UnkToggle_ovb0062*)>(vtable[0x110 / 8])(this); }
    void vf_0x108() { reinterpret_cast<void (*)(UnkToggle_ovb0062*)>(vtable[0x108 / 8])(this); }

    bool IsActive() const {
        if (!enabled || !owner || !owner->field_0x1b4)
            return false;
        return !owner->field_0x1b0 || owner->field_0x1a8 != nullptr;
    }
};

// Re-reads the enabled flag (vtable slot 0x120); when it changes and with it whether the component
// is active (enabled, with an owner whose flag at +0x1b4 is set and that either has no flag at
// +0x1b0 or an object at +0x1a8), slot 0x108 (now active) or 0x110 (no longer) is called. Both
// values then restart from the source's current one (as fn_710024f8ac,
// src/base/UnkUnit342/UnkUnit342_Attach_b66a1.cpp).
void fn_710024f918(UnkToggle_ovb0062* self) {
    bool enabled = self->vf_0x120();
    if (self->enabled != enabled) {
        bool before = self->IsActive();
        self->enabled = enabled;
        bool after = self->IsActive();
        if (before != after) {
            void** vtable = self->vtable;
            if (!before)
                reinterpret_cast<void (*)(UnkToggle_ovb0062*)>(vtable[0x108 / 8])(self);
            else
                reinterpret_cast<void (*)(UnkToggle_ovb0062*)>(vtable[0x110 / 8])(self);
        }
    }
    f32 value = fn_7100253ad4(self->source);
    self->target = value;
    self->current = value;
}
