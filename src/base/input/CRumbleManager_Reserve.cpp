// From unit 51 of tools/tu_map.py (base::input::CRumbleManager's unit): grows a named registry
// (base::UnkNamedRegistry, base/UnkNamedRegistry.h: object pointers at +0 and their keys, strings
// with base::global::CRntString's layout, at +8, in parallel) to hold at least `capacity` entries.
// The object pointers are copied over bitwise, the keys moved (fn_710007eab4) and destroyed; the
// old arrays are freed (fn_710007547c). The header's layout stops before the allocation flags and
// the allocator, so the registry is viewed through the full layout here. Names are placeholders.
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkKeyedStrings_710007547c {
    base::UnkNamedObject** keys;
    CRntString* values;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710007547c(base::UnkNamedRegistry* registry, u32 capacity) {
    auto* self = reinterpret_cast<UnkKeyedStrings_710007547c*>(registry);
    if (self->capacity >= capacity)
        return;
    auto* keys = static_cast<base::UnkNamedObject**>(
        self->allocator->vf_0x40((u64)capacity * sizeof(base::UnkNamedObject*), 8, self->flags));
    auto* values = static_cast<CRntString*>(self->allocator->vf_0x40((u64)capacity * sizeof(CRntString), 8, self->flags));
    if (self->keys) {
        memcpy(keys, self->keys, (u64)self->count * sizeof(base::UnkNamedObject*));
        self->allocator->vf_0x48(self->keys);
    }
    if (CRntString* old = self->values) {
        CRntString* dst = values;
        for (u32 i = 0; i < self->count; ++i) {
            fn_710007eab4(dst, old);
            fn_710007ecd4(old);
            ++dst;
            ++old;
        }
        self->allocator->vf_0x48(self->values);
    }
    self->keys = keys;
    self->values = values;
    self->capacity = capacity;
}
