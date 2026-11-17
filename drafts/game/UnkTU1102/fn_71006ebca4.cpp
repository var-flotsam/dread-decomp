// Unit 1102 (CXParasiteAIComponent): grows the table of state bindings (values and u32 keys) to hold
// `capacity` entries; each binding is moved and the old one destroyed, the keys copied. Names and
// layouts are placeholders.
#include <new>
#include <string.h>

#include "base/global/CRntSmallDictionary.h"
#include "game/UnkXParasiteAI.h"
#include "types.h"

using UnkTU1102BindingTable = base::global::CRntSmallDictionary<u32, UnkXParasiteBinding>;

void fn_71006ebca4(UnkTU1102BindingTable* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkXParasiteBinding* values = static_cast<UnkXParasiteBinding*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkXParasiteBinding), 8, self->flags));
        u32* keys = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
        if (UnkXParasiteBinding* old = self->values) {
            UnkXParasiteBinding* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                new (dst) UnkXParasiteBinding(static_cast<UnkXParasiteBinding&&>(*old));
                old->~UnkXParasiteBinding();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            memcpy(keys, self->keys, self->count * sizeof(u32));
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
