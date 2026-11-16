// CSpawnPointComponent's unit (unit 2704): inserts a copy of a holder (0x40 bytes: vtable, callback vector
// at +0x8, state at +0x30, refcounted handle at +0x38) at an index of the holder array, moving the later
// entries up one place (growing the array first when it is full); appending is fn_7101131b98.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkSpawnHolder.h"
#include "game/UnkTU2704.h"
#include "types.h"

void fn_7101131ea8(base::global::CRntVector<UnkSpawnHolder>* self, const UnkSpawnHolder* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7101131b98(self, value);
        return;
    }
    if (self->capacity <= n && self->step != 0) {
        u32 capacity = self->GrownCapacity(n);
        if (self->capacity < capacity) {
            UnkSpawnHolder* fresh = static_cast<UnkSpawnHolder*>(
                self->allocator->vf_0x40((u64)capacity * sizeof(UnkSpawnHolder), 8, self->flags));
            if (UnkSpawnHolder* src = self->data) {
                UnkSpawnHolder* dst = fresh;
                for (u32 i = 0; i < self->count; ++i) {
                    fn_710111fd80(dst, src);
                    (src++)->~UnkSpawnHolder();
                    ++dst;
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = fresh;
            self->capacity = capacity;
        }
    }
    // The entries from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = n - 1; i >= index; --i) {
        UnkSpawnHolder* from = &self->data[i];
        fn_710111fd80(from + 1, from);
        self->data[i].~UnkSpawnHolder();
        if (i == 0)
            break;
    }
    UnkSpawnHolder* e = &self->data[index];
    new (e) UnkSpawnHolder;
    e->CopyFrom(*value);
    ++self->count;
}
