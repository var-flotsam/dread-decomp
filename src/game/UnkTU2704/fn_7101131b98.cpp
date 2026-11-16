// CSpawnPointComponent unit (tools/tu_map.py, unit 2704): appends a copy of a lazily resolved holder (0x40
// bytes: vtable, callback vector, state, refcounted handle) to a base::global::CRntVector of them, growing the array
// by moving the entries (fn_710111fd80). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkSpawnHolder.h"
#include "game/UnkTU2704.h"
#include "types.h"

void fn_7101131b98(base::global::CRntVector<UnkSpawnHolder>* self, const UnkSpawnHolder* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkSpawnHolder* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkSpawnHolder*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkSpawnHolder), 8, self->flags));
        if (UnkSpawnHolder* src = self->data) {
            UnkSpawnHolder* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_710111fd80(dst, src);
                (src++)->~UnkSpawnHolder();
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    UnkSpawnHolder* e = &p[n];
    new (e) UnkSpawnHolder;
    e->CopyFrom(*value);
    ++self->count;
}
